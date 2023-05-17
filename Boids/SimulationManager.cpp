#include "SimulationManager.hpp"
#include <cstddef>
#include <vector>
#include "GLFW/glfw3.h"

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
    const glm::vec3& pos1 = entity1.getPosition();
    const glm::vec3& pos2 = entity2.getPosition();
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
                return myDistance(currentObstacle, other) < parameters.distance + currentObstacle.getSize();
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

void firingManager(std::vector<Laser>& lasers, const LaserParameters parameters, const p6::Context& ctx, const CharacterCamera& camera)
{
    if (ctx.key_is_pressed(GLFW_KEY_SPACE))
    {
        lasers.emplace_back(camera.getPosition(), parameters, camera.getFrontVector());
        std::cout << lasers.size() << std::endl;
    }
}

void lasersManager(std::vector<Laser>& lasers, const std::vector<Obstacle>& obstacles, std::vector<Boid>& boids)
{
    // Boids erased if in the field of a laser
    for (const Laser& laser : lasers)
    {
        const float laserField = laser.getRange();
        boids.erase(std::remove_if(boids.begin(), boids.end(), [&laser, laserField](const Boid& boid) {
                        const float distance = myDistance(laser, boid);
                        return distance < laserField;
                    }),
                    boids.end());
    }

    // Lasers erased if out of border or inside an obstacle
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(), [&obstacles](const Laser& laser) {
                     return laser.outOfBorders()
                            || std::any_of(obstacles.begin(), obstacles.end(), [&laser](const Obstacle& obstacle) {
                                   return myDistance(laser, obstacle) < obstacle.getSize();
                               });
                 }),
                 lasers.end());
}

void lasersDisplacement(std::vector<Laser>& lasers, const float deltaTime)
{
    for (Laser laser : lasers)
    {
        laser.displacement(deltaTime);
    }
}
