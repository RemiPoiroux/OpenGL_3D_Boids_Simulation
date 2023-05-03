#include "BoidsManager.hpp"
#include <vector>
#include "Boid.hpp"
#include "Obstacle.hpp"

/////////////////////////////////
// PARAMETERS

const float MAX_SPEED_MIN = 0.15;
const float MAX_SPEED_MAX = 0.2;

/////////////////////////////////

glm::vec3 RandomDirection()
{
    glm::vec3 vec = RandomVec3(1);
    normaliseVector(vec);
    return vec;
}

Boid randomBoid()
{
    return {RandomVec3(1), RandomFloat(MAX_SPEED_MIN, MAX_SPEED_MAX), RandomDirection()};
}

template<typename T1, typename T2>
float myDistance(const T1& entity1, const T2& entity2)
{
    const glm::vec3& pos1 = entity1.pos();
    const glm::vec3& pos2 = entity2.pos();
    return glm::length(pos2 - pos1);
}

std::vector<Boid> createBoids(const size_t nb)
{
    std::vector<Boid> boids;
    boids.reserve(nb);
    std::generate_n(std::back_inserter(boids), nb, [] { return randomBoid(); });
    return boids;
}

void neighborsManager(std::vector<Boid>& boids, const NeighborsParameters parameters)
{
    for (size_t i = 0; i < boids.size(); ++i)
    {
        Boid* currentBoid = &boids[i];
        float distance    = parameters.alignment.distance;

        auto applyToCloseBoids = [&](const auto& func) {
            auto isClose = [&](const Boid& other) {
                return myDistance(*currentBoid, other) < distance;
            };
            auto closeBoidsBegin = std::find_if(std::begin(boids), std::end(boids), isClose);
            auto closeBoidsEnd   = std::find_if_not(closeBoidsBegin, std::end(boids), isClose);
            for (auto boid = closeBoidsBegin; boid != closeBoidsEnd; ++boid)
            {
                func(*boid);
            }
        };

        applyToCloseBoids([&](const Boid& other) {
            currentBoid->neighborsAlignement(other, parameters.alignment.strength);
        });

        distance = parameters.cohesion.distance;
        applyToCloseBoids([&](const Boid& other) {
            currentBoid->neighborsCohesion(other, parameters.cohesion.strength);
        });

        distance = parameters.separation.distance;
        applyToCloseBoids([&](const Boid& other) {
            currentBoid->neighborsSeparation(other, parameters.separation.strength);
        });
    }
}

void obstaclesManager(std::vector<Boid>& boids, const std::vector<Obstacle>& obstacles, const Parameters parameters)
{
    for (Obstacle currentObstacle : obstacles)
    {
        auto applyToCloseBoids = [&](const auto& func) {
            auto isClose = [&](Boid& other) {
                return myDistance(currentObstacle, other) < parameters.distance + currentObstacle.size();
            };
            auto closeBoidsBegin = std::find_if(std::begin(boids), std::end(boids), isClose);
            auto closeBoidsEnd   = std::find_if_not(closeBoidsBegin, std::end(boids), isClose);
            for (auto boid = closeBoidsBegin; boid != closeBoidsEnd; ++boid)
            {
                func(*boid);
            }
        };

        applyToCloseBoids([&](Boid& other) {
            other.obstacleAvoidance(currentObstacle, parameters.strength);
        });
    }
}

void borderManager(std::vector<Boid>& boids, const Parameters parameters)
{
    for (Boid& boid : boids)
    {
        boid.bordersAvoidance(parameters);
    }
}

void boidsDisplacement(std::vector<Boid>& boids, const float deltaTime)
{
    for (Boid& boid : boids)
    {
        boid.acceleration();
        boid.displacement(deltaTime);
    }
}