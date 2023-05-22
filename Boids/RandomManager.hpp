#pragma once

#include "Boid.hpp"
#include "RandomVariables.hpp"

struct RandomVariablesParameters;

struct RandomVariables
{
    ExponentialRandomVariable            obstacleSizesVar;
    PoissonRandomVariable                boidsLivesVar;
    BernoulliRandomVariable              collisionWithObstaclesVar;
    DiscreteRandomVariable<BoidBehavior> boidsAttitudeVar;
    BinomialRandomVariable               boidsFiringVar;
    NormalRandomVariable                 boidsPrecisionVar;
    GeometricRandomVariable              characterFiringVar;
};

RandomVariables initializeRandomVariables(const RandomVariablesParameters& parameters);

struct ExponentialRandomParameters
{
    float sizeExpectation;
};
struct PoissonRandomParameters
{
    float livesExpectation;
};
struct BernoulliRandomParameters
{
    float collisionProb;
};
struct DiscreteRandomParameters
{
    std::vector<BoidBehavior> stateNames;
    std::vector<float>        stateProbs;
};
struct BinomialRandomParameters
{
    float shootProbPerFrame;
    float secondsToShoot;
};
struct NormalRandomParameters
{
    float inaccuracyExpectation;
};
struct GeometricRandomParameters
{
    float shootProbPerFrame;
};

struct RandomVariablesParameters
{
    ExponentialRandomParameters obstaclesSizes{};
    PoissonRandomParameters     boidsLifeVar{};
    BernoulliRandomParameters   collisionWithObstacles{};
    DiscreteRandomParameters    boidsAttitude;
    BinomialRandomParameters    boidsFiring{};
    NormalRandomParameters      boidsAccuracy{};
    GeometricRandomParameters   characterFiring{};
};