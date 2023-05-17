#include "CameraInputs.hpp"
#include "CharacterCamera.hpp"
#include "ImGuiInterface.hpp"
#include "Laser.hpp"
#include "Render.hpp"
#include "SimulationManager.hpp"

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
    bool SPOT_LIGHT = false;

    // SIMULATION PARAMETERS

    const int NB_BOIDS          = 200;
    bool      BOIDS_LOW_QUALITY = false;

    Parameters BORDERS_PARAMETERS = {0.2, 0.02f};

    NeighborsParameters NEIGHBORS_PARAMETERS =
        {{0.35f, 0.02f},
         {0.25f, 0.002f},
         {0.1f, 0.2f}};

    ObstaclesParameters OBSTACLES_PARAMETERS =
        {100, 0.05, 0.8, {0.1, 0.1}};

    LaserParameters LASERS_PARAMETERS = {1, 0.01};

    /////////////////////////////////
    /////////////////////////////////

    int  width  = 1280;
    int  height = 720;
    auto ctx    = p6::Context{{width, height, "Remi's Boids"}};
    ctx.maximize_window();

    // Initialize depth test
    glEnable(GL_DEPTH_TEST);

    // Initialize camera
    CharacterCamera camera;

    // Initialize Boids, obstacles and lasers
    std::vector<Boid>     boids     = createBoids(NB_BOIDS);
    std::vector<Obstacle> obstacles = createObstacles(OBSTACLES_PARAMETERS);
    std::vector<Laser>    lasers{};

    // Initialize Render Matrices
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(viewField), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.01f, 3.f);

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
        ImGuiInterface(BOIDS_LOW_QUALITY, OBSTACLES_PARAMETERS.force, NEIGHBORS_PARAMETERS, BORDERS_PARAMETERS, LASERS_PARAMETERS);

        cameraInputsEvents(ctx, CAM_PARAMETERS, camera);

        // Boids simulation
        firingManager(lasers, LASERS_PARAMETERS, ctx, camera);
        lasersManager(lasers, obstacles, boids);
        neighborsManager(boids, NEIGHBORS_PARAMETERS);
        obstaclesManager(boids, obstacles, OBSTACLES_PARAMETERS.force);
        borderManager(boids, BORDERS_PARAMETERS);
        boidsDisplacement(boids, ctx.delta_time());
        lasersDisplacement(lasers, ctx.delta_time());

        render(ctx, boids, obstacles, BOIDS_LOW_QUALITY, vaos, camera, textures, ProjMatrix, SPOT_LIGHT);
    };

    /////////////////////////////////
    /////////////////////////////////

    // Should be done last. It starts the infinite loop.
    ctx.start();

    releasesBuffers(vbos, ibos, vaos, textures);
}