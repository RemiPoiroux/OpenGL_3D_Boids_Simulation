#include "ImGuiInterface.hpp"
#include "Inputs.hpp"
#include "MyBuffers.hpp"
#include "Obstacle.hpp"

int main()
{
    /////////////////////////////////
    /////////////////////////////////

    // CAMERA PARAMETERS

    const float      viewField = 70.f;
    CameraParameters CAM_PARAMETERS =
        {0.2,
         2,
         50,
         10000};

    // SIMULATION PARAMETERS

    const int NB_BOIDS      = 200;
    float     BOIDS_QUALITY = 1;

    Parameters BORDERS_PARAMETERS = {0.2, 0.02f};

    NeighborsParameters NEIGHBORS_PARAMETERS =
        {{0.35f, 0.02f},
         {0.25f, 0.002f},
         {0.1f, 0.5f}};

    ObstaclesParameters OBSTACLES_PARAMETERS =
        {10, 0.05, 0.2, 0.2};

    /////////////////////////////////
    /////////////////////////////////

    glEnable(GL_DEPTH_TEST);

    int  width  = 1280;
    int  height = 720;
    auto ctx    = p6::Context{{width, height, "Remi's Boids"}};
    ctx.maximize_window();

    // Initialize Render Matrices
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(viewField), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.01f, 3.f);
    glm::mat4 ViewMatrix;

    FreeflyCamera camera;

    std::vector<Boid>     boids     = createBoids(NB_BOIDS);
    std::vector<Obstacle> obstacles = createObstacles(OBSTACLES_PARAMETERS);

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
        neighborsManager(boids, NEIGHBORS_PARAMETERS);
        borderManager(boids, BORDERS_PARAMETERS);
        boidsDisplacement(boids, ctx.delta_time());

        ViewMatrix = camera.getViewMatrix();
        render(ctx, boids, vaos, ViewMatrix, textures, ProjMatrix);
    };

    /////////////////////////////////
    /////////////////////////////////

    // Should be done last. It starts the infinite loop.
    ctx.start();

    releasesRessources(vbos, ibos, vaos, textures);
}