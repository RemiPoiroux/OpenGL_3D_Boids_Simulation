#include "Boid.hpp"
#include "CameraInputs.hpp"
#include "CharacterCamera.hpp"
#include "ImGuiInterface.hpp"
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "Render.hpp"
#include "SimulationManager.hpp"

int main()
{
    /////////////////////////////////
    /////////////////////////////////

    // CAMERA PARAMETERS

    const float      viewField = 70.f;
    const uint       livesNb   = 300;
    CameraParameters CAM_PARAMETERS =
        {0.2,
         2,
         50,
         10000};
    bool SPOT_LIGHT = false;

    // SIMULATION PARAMETERS

    int boidsNumber = 200;

    ObstaclesParameters OBSTACLES_PARAMETERS =
        {100, 0.05, 0.5, {0.1, 0.1}};

    LodsParameters LODS_PARAMETERS = {false, 1};

    Parameters BORDERS_FORCE = {0.2, 0.02f};

    Parameters CHARACTER_FORCE{0.3f, 0.2f};

    NeighborsParameters NEIGHBORS_PARAMETERS =
        {{0.35f, 0.02f},
         {0.25f, 0.002f},
         {0.1f, 0.2f}};

    LaserParameters LASERS_PARAMETERS = {2, 0.04};
    LaserDelays     laserDelays{};

    RandomVariablesParameters RANDOM_VARIABLES_PARAMETERS =
        {{(OBSTACLES_PARAMETERS.maxSize + OBSTACLES_PARAMETERS.minSize) / 2},
         {10},
         {0.3},
         {{BoidBehavior::Attacks, BoidBehavior::Neutral, BoidBehavior::Flees},
          {0.3, 0.5, 0.2}},
         {0.3, 0.5},
         {0.2},
         {0.03}};

    /////////////////////////////////
    /////////////////////////////////

    int  width  = 1280;
    int  height = 720;
    auto ctx    = p6::Context{{width, height, "Remi's Boids"}};
    ctx.maximize_window();
    glEnable(GL_DEPTH_TEST);

    // Initialize probabilities
    RandomVariables randomVariables = initializeRandomVariables(RANDOM_VARIABLES_PARAMETERS);

    // Initialize camera, boids, obstacles and lasers
    CharacterCamera       camera(livesNb);
    std::vector<Obstacle> obstacles = createObstacles(OBSTACLES_PARAMETERS, randomVariables.obstacleSizesVar);
    std::vector<Boid>     boids     = createBoids(boidsNumber, randomVariables.boidsLivesVar, obstacles);
    std::vector<Laser>    lasers{};

    // Initialize Render Matrices
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(viewField), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.01f, 3.f);

    // Initialize Maps of Buffers
    MyBuffers vbos{};
    MyBuffers ibos{};
    MyBuffers vaos{};
    MyBuffers textures{};
    initializesBuffers(vbos, ibos, vaos, textures);

    bool victory = false;

    /////////////////////////////////
    /////////////////////////////////

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Victory or defeat check
        if (boids.empty() || camera.getLives() < 1)
        {
            ctx.resume();
        }

        gameImGuiInterface(LODS_PARAMETERS, OBSTACLES_PARAMETERS.force, NEIGHBORS_PARAMETERS, BORDERS_FORCE, LASERS_PARAMETERS, camera.getLives(), CHARACTER_FORCE, static_cast<int>(boids.size()), RANDOM_VARIABLES_PARAMETERS, randomVariables);

        cameraInputsEvents(ctx, CAM_PARAMETERS, camera);

        // Shooting simulation
        lasersDisplacement(lasers, ctx.delta_time());
        characterFiringManager(lasers, LASERS_PARAMETERS, ctx, camera, randomVariables.characterFiringVar, laserDelays);
        boidsBehaviorManager(camera, boids, CHARACTER_FORCE, randomVariables.boidsAttitudeVar, SPOT_LIGHT);
        boidsFiringManager(lasers, boids, LASERS_PARAMETERS, camera.getPosition(), randomVariables.boidsFiringVar, RANDOM_VARIABLES_PARAMETERS.boidsFiring, randomVariables.boidsPrecisionVar);
        lasersManager(lasers, obstacles, boids, camera);

        // Boids simulation
        neighborsManager(boids, NEIGHBORS_PARAMETERS);
        obstaclesManager(boids, obstacles, OBSTACLES_PARAMETERS.force, randomVariables.collisionWithObstaclesVar);
        borderManager(boids, BORDERS_FORCE);
        boidsDisplacement(boids, ctx.delta_time());

        render(ctx, boids, obstacles, lasers, LODS_PARAMETERS, vaos, camera, textures, ProjMatrix, SPOT_LIGHT);

        // Victory or defeat check
        if (boids.empty())
        {
            victory = true;
            endImGuiInterface(victory, randomVariables);
        }
        if (camera.getLives() < 1)
        {
            endImGuiInterface(victory, randomVariables);
        }
    };

    /////////////////////////////////
    /////////////////////////////////

    // Should be done last. It starts the infinite loop.
    ctx.start();

    releasesBuffers(vbos, ibos, vaos, textures);
}