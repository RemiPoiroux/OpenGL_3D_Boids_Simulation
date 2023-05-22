#include "RandomManager.hpp"
#include "RandomVariables.hpp"

RandomVariables initializeRandomVariables(const RandomVariablesParameters& parameters)
{
    RandomVariables variables = {
        ExponentialRandomVariable(parameters.obstaclesSizes.sizeExpectation),
        PoissonRandomVariable(parameters.boidsLifeVar.livesExpectation),
        BernoulliRandomVariable(parameters.collisionWithObstacles.collisionProb),
        DiscreteRandomVariable(parameters.boidsAttitude.stateProbs, parameters.boidsAttitude.stateNames),
        BinomialRandomVariable(parameters.boidsFiring.shootProbPerFrame, static_cast<uint>(parameters.boidsFiring.secondsToShoot * 60)),
        NormalRandomVariable(parameters.boidsAccuracy.inaccuracyExpectation),
        GeometricRandomVariable(parameters.characterFiring.shootProbPerFrame)};

    return variables;
}