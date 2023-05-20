#pragma once

#include "Boid.hpp"
#include "CharacterCamera.hpp"
#include "Laser.hpp"
#include "Obstacle.hpp"
#include "RandomManager.hpp"
struct NeighborsParameters
{
    Parameters alignment;

    Parameters cohesion;

    Parameters separation;
};

struct BoidsParameters
{
    int   number;
    float livesExpectation;
};

struct LaserDelays
{
    std::vector<int> topLeft;
    std::vector<int> topRight;
    std::vector<int> botLeft;
    std::vector<int> botRight;
};

template<typename T1, typename T2>
float distance(const T1& entity1, const T2& entity2);

std::vector<Boid> createBoids(BoidsParameters boidsParameters);

void neighborsManager(std::vector<Boid>& boids, NeighborsParameters parameters);

void obstaclesManager(std::vector<Boid>& boids, const std::vector<Obstacle>& obstacles, Parameters parameters, const BernoulliRandomVariable& var);

void boidsBehaviorManager(const CharacterCamera& camera, std::vector<Boid>& boids, Parameters characterForce, const DiscreteRandomVariable<BoidBehavior>& behaviorVar, bool spotLight);

void boidsFiringManager(std::vector<Laser>& lasers, std::vector<Boid> boids, LaserParameters parameters, glm::vec3 characterPosition, const BinomialRandomVariable& fireVar, const BinomialRandomParameters& fireParam, const NormalRandomVariable& accuracyVar);

void characterFiringManager(std::vector<Laser>& lasers, LaserParameters parameters, const p6::Context& ctx, const CharacterCamera& camera, const GeometricRandomVariable& var, LaserDelays& delays);

void lasersManager(std::vector<Laser>& lasers, const std::vector<Obstacle>& obstacles, std::vector<Boid>& boids, CharacterCamera& camera);

void borderManager(std::vector<Boid>& boids, Parameters parameters);

void boidsDisplacement(std::vector<Boid>& boids, float deltaTime);

void lasersDisplacement(std::vector<Laser>& lasers, float deltaTime);
