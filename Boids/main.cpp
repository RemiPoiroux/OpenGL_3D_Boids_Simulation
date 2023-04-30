#include "ImGuiInterface.hpp"
#include "Inputs.hpp"
#include "MyBuffers.hpp"

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

    const int NB_BOIDS      = 50;
    float     BOIDS_QUALITY = 1;

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

    // Initialize depth test
    glEnable(GL_DEPTH_TEST);

    // Initialize camera
    FreeflyCamera camera;

    // Initialize Boids
    std::vector<Boid> boids = createBoids(NB_BOIDS);

    // Initialize Render Matrices
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.f);
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
        ImGuiInterface(BOIDS_QUALITY, NEIGHBORS_PARAMETERS, BORDERS_PARAMETERS);

        inputsEvents(ctx, CAM_PARAMETERS, camera);

        // Boids simulation
        // neighborsManager(boids, NEIGHBORS_PARAMETERS);
        borderManager(boids, BORDERS_PARAMETERS);
        boidsDisplacement(boids);

        ViewMatrix = camera.getViewMatrix();
        render(boids, vaos, ViewMatrix, textures, ProjMatrix);
    };

    /////////////////////////////////
    /////////////////////////////////

    // Should be done last. It starts the infinite loop.
    ctx.start();

    releasesRessources(vbos, ibos, vaos, textures);
}