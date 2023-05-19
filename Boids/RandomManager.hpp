#pragma once

#include "RandomVariables.hpp"

struct RandomVariablesParameters;

struct RandomVariables
{
    BernoulliRandomVariable collisionWithObstaclesVar;
    DiscreteRandomVariable  boidsAttitudeVar;
    BinomialRandomVariable  boidsFiringVar;
    NormalRandomVariable    boidsPrecisionVar;
    GeometricRandomVariable characterFiringVar;
};

RandomVariables initializeRandomVariables(const RandomVariablesParameters& parameters);

struct BernoulliRandomParameters
{
    float collisionProb;
};
struct DiscreteRandomParameters
{
    std::vector<std::string> stateNames;
    std::vector<float>       stateProbs;
};
struct BinomialRandomParameters
{
    float shootProbPerFrame;
    float secondsToShoot;
};
struct NormalRandomParameters
{
    float accuracyExpectation;
};
struct GeometricRandomParameters
{
    float shootProbPerFrame;
};

struct RandomVariablesParameters
{
    BernoulliRandomParameters collisionWithObstacles{};
    DiscreteRandomParameters  boidsAttitude;
    BinomialRandomParameters  boidsFiring{};
    NormalRandomParameters    boidsAccuracy{};
    GeometricRandomParameters characterFiring{};
};