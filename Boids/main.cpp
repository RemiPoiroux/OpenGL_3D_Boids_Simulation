#include <ostream>
#include "BoidsManager.hpp"
#include "FreeflyCamera.hpp"
#include "Programs.hpp"
#include "Vbos&Ibos.hpp"

int main()
{
    /////////////////////////////////

    // CAMERA PARAMETERS

    float CAM_TRANSLATION_FORCE = 0.001;
    float CAM_ROTATION_FORCE    = 0.1;
    float MOUSE_ROTATION_FORCE  = 50;
    float FAST_SPEED_FACTOR     = 10;

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

    GLuint vbos{};
    glGenBuffers(1, &vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos);
    std::vector<glimac::ShapeVertex> backgroundVertices = cubeVbo();
    glBufferData(GL_ARRAY_BUFFER, backgroundVertices.size() * sizeof(glimac::ShapeVertex), backgroundVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibos{};
    glGenBuffers(1, &ibos);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos);
    std::vector<uint32_t> backgroundIndices = cubeIbo();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, backgroundIndices.size() * sizeof(uint32_t), backgroundIndices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vaos{};
    glGenVertexArrays(1, &vaos);
    glBindVertexArray(vaos);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos);

    static constexpr GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    static constexpr GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    static constexpr GLuint VERTEX_ATTR_TEXTURE = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

    glBindBuffer(GL_ARRAY_BUFFER, vbos);
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), nullptr);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Initialize textures
    img::Image backgroundImage = p6::load_image_buffer("assets/textures/starry-night-sky.jpg");

    GLuint backgroundTexture{};

    GLuint textures[] = {backgroundTexture};

    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, backgroundImage.width(), backgroundImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, backgroundImage.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*********************************/

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Events
        ImGui::Begin("Control");
        ImGui::SliderFloat("Boids size", &BOIDS_SIZE, 0.01f, 1.f);
        ImGui::SliderFloat("Alignment distance", &NEIGHBORS_PARAMETERS.alignment.distance, 0.f, 2.f);
        ImGui::SliderFloat("Alignment strength", &NEIGHBORS_PARAMETERS.alignment.strength, 0.f, 1.f);
        ImGui::SliderFloat("Cohesion distance", &NEIGHBORS_PARAMETERS.cohesion.distance, 0.f, 2.f);
        ImGui::SliderFloat("Cohesion strength", &NEIGHBORS_PARAMETERS.cohesion.strength, 0.f, 1.f);
        ImGui::SliderFloat("Separation distance", &NEIGHBORS_PARAMETERS.separation.distance, 0.f, 2.f);
        ImGui::SliderFloat("Separation strength", &NEIGHBORS_PARAMETERS.separation.strength, 0.f, 1.f);
        ImGui::SliderFloat("Borders distance", &BORDERS_PARAMETERS.distance, 0.f, 1.f);
        ImGui::SliderFloat("Borders strength", &BORDERS_PARAMETERS.strength, 0.f, 1.f);
        ImGui::End();

        // camera move
        float factor = 1;
        if (ctx.shift())
        {
            factor = FAST_SPEED_FACTOR;
        };

        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera.moveLeft(factor * CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera.moveLeft(-factor * CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            camera.moveFront(factor * CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera.moveFront(-factor * CAM_TRANSLATION_FORCE);
        }

        // camera rotation
        if (ctx.key_is_pressed(GLFW_KEY_Q))
        {
            camera.rotateLeft(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_E))
        {
            camera.rotateLeft(-CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_LEFT))
        {
            camera.rotateLeft(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_RIGHT))
        {
            camera.rotateLeft(-CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_UP))
        {
            camera.rotateUp(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_DOWN))
        {
            camera.rotateUp(-CAM_ROTATION_FORCE);
        }
        if (ctx.mouse_moved)
        {
            if (ctx.alt() || ctx.mouse_button_is_pressed(p6::Button::Right))
            {
                camera.rotateLeft(-ctx.mouse_delta().x * MOUSE_ROTATION_FORCE);
                camera.rotateUp(ctx.mouse_delta().y * MOUSE_ROTATION_FORCE);
            }
        }

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
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glBindVertexArray(vaos);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibos);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(backgroundIndices.size()), GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        /*********************************/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Ressources released
    glDeleteBuffers(1, &vbos);
    glDeleteBuffers(1, &ibos);
    glDeleteVertexArrays(1, &vaos);
    glDeleteTextures(1, textures);
}