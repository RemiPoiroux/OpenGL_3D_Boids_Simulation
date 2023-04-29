#include "ImGuiInterface.hpp"
#include "Inputs.hpp"
#include "MyBuffers.hpp"
#include "Programs.hpp"

void ReleasesRessources(Vbos& vbos, Ibos& ibos, Vaos& vaos, Textures& textures)
{
    glDeleteBuffers(1, &vbos.backgroundVbo);
    glDeleteBuffers(1, &ibos.backgroundIbo);
    glDeleteVertexArrays(1, &vaos.backgroundVao);
    glDeleteTextures(1, &textures.backgroundTex);
}

int main()
{
    /////////////////////////////////

    // CAMERA PARAMETERS

    CameraParameters CAM_PARAMETERS =
        {0.001,
         10,
         0.1,
         50};

    // SIMULATION PARAMETERS

    const int NB_BOIDS   = 20;
    float     BOIDS_SIZE = 0.04;

    Parameters BORDERS_PARAMETERS = {0.2, 0.3};

    NeighborsParameters NEIGHBORS_PARAMETERS =
        {{0.4f, 0.005f},
         {0.3f, 0.002f},
         {0.1f, 0.5f}};

    /////////////////////////////////

    int  width  = 1280;
    int  height = 720;
    auto ctx    = p6::Context{{width, height, "Remi's Boids"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    // Initialize depth test
    glEnable(GL_DEPTH_TEST);

    // Initialise Programs
    BackgroundProgram backgroundProgram = BackgroundProgram();

    // Initialize camera
    FreeflyCamera camera;

    // Initialize Boids
    std::vector<Boid> boids = createBoids(NB_BOIDS);

    // Initialize Render Matrix
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.f);
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 ViewMatrix;

    Vbos vbos{};
    glGenBuffers(1, &vbos.backgroundVbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbos.backgroundVbo);
    std::vector<glimac::ShapeVertex> backgroundVertices = cubeVbo();
    glBufferData(GL_ARRAY_BUFFER, backgroundVertices.size() * sizeof(glimac::ShapeVertex), backgroundVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Ibos ibos{};
    glGenBuffers(1, &ibos.backgroundIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos.backgroundIbo);
    std::vector<uint32_t> backgroundIndices = cubeIbo();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, backgroundIndices.size() * sizeof(uint32_t), backgroundIndices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Vaos vaos{};
    glGenVertexArrays(1, &vaos.backgroundVao);
    glBindVertexArray(vaos.backgroundVao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos.backgroundIbo);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    static constexpr GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbos.backgroundVbo);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), nullptr);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Initialize textures
    img::Image backgroundImage = p6::load_image_buffer("assets/textures/starry-night-sky.jpg");

    Textures textures{};

    glGenTextures(1, &textures.backgroundTex);
    glBindTexture(GL_TEXTURE_2D, textures.backgroundTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundImage.width(), backgroundImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, backgroundImage.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*********************************/

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ImGuiInterface(BOIDS_SIZE, NEIGHBORS_PARAMETERS, BORDERS_PARAMETERS);

        inputsEvents(ctx, CAM_PARAMETERS, camera);

        // Boids simulation
        neighborsManager(boids, NEIGHBORS_PARAMETERS);

        borderManager(boids, BORDERS_PARAMETERS);

        boidsDisplacement(boids);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ViewMatrix = camera.getViewMatrix();

        backgroundProgram.use();
        glUniform1i(backgroundProgram.uTexture(), 0);

        MVMatrix     = glm::mat4(1);
        MVMatrix     = ViewMatrix * MVMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(backgroundProgram.uMVMatrix(), 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(backgroundProgram.uMVPMatrix(), 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(backgroundProgram.uNormalMatrix(), 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures.backgroundTex);

        glBindVertexArray(vaos.backgroundVao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(backgroundIndices.size()), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        /*********************************/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Ressources released
    ReleasesRessources(vbos, ibos, vaos, textures);
}