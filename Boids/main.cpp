#include "ImGuiInterface.hpp"
#include "Inputs.hpp"
#include "MyBuffers.hpp"
#include "Programs.hpp"

int main()
{
    /////////////////////////////////
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

    // Initialize Maps of Buffers
    MyBuffers vbos{};
    MyBuffers ibos{};
    MyBuffers vaos{};
    MyBuffers textures{};
    initializesBuffers(vbos, ibos, vaos, textures);

    /////////////////////////////////
    /////////////////////////////////

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
        glBindTexture(GL_TEXTURE_2D, textures.m_["background"]);

        glBindVertexArray(vaos.m_["background"]);
        GLint size{};
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        /*********************************/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    releasesRessources(vbos, ibos, vaos, textures);
}