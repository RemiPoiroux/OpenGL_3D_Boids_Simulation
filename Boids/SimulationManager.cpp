#include "SimulationManager.hpp"
#include <cstddef>
#include <vector>
#include "Boid.hpp"
#include "CharacterCamera.hpp"
#include "GLFW/glfw3.h"
#include "Obstacle.hpp"
#include "RandomManager.hpp"
#include "RandomVariables.hpp"
#include "glm/ext/quaternion_geometric.hpp"

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

Boid randomBoid(PoissonRandomVariable& livesVar)
{
    int lives = static_cast<int>(livesVar.generate());
    return {RandomVec3(1), RandomFloat(MAX_SPEED_MIN, MAX_SPEED_MAX), RandomDirection(), lives};
}

template<typename T1, typename T2>
float myDistance(const T1& entity1, const T2& entity2)
{
    const glm::vec3& pos1 = entity1.getPosition();
    const glm::vec3& pos2 = entity2.getPosition();
    return glm::length(pos2 - pos1);
}

std::vector<Boid> createBoids(const int boidsNb, PoissonRandomVariable& livesVar, const std::vector<Obstacle>& obstacles)
{
    std::vector<Boid> boids;
    boids.reserve(boidsNb);

    auto checkOverlap = [&obstacles](Boid boid) {
        return std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& o) {
            double squaredDistance = pow(boid.getPosition().x - o.getPosition().x, 2) + pow(boid.getPosition().y - o.getPosition().y, 2);
            return squaredDistance < pow(o.getSize() / 2 + 0.1, 2);
        });
    };

    std::generate_n(std::back_inserter(boids), boidsNb, [&]() {
        Boid newBoid = randomBoid(livesVar);
        ;
        while (checkOverlap(newBoid))
        {
            newBoid = randomBoid(livesVar);
        };
        return newBoid;
    });

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

void obstaclesManager(std::vector<Boid>& boids, const std::vector<Obstacle>& obstacles, const Parameters parameters, BernoulliRandomVariable& var)
{
    for (Obstacle currentObstacle : obstacles)
    {
        auto removeBoidIfNecessary = [&](Boid& boid) {
            if (static_cast<bool>(var.generate()))
            {
                auto it = std::find_if(std::begin(boids), std::end(boids), [&](const Boid& other) { return &other == &boid; });
                if (it != std::end(boids))
                {
                    boids.erase(it);
                }
            }
            else
            {
                boid.obstacleAvoidance(currentObstacle, 2);
            }
        };
        auto applyToBoidsInside = [&](const auto& func) {
            auto isClose = [&](Boid& other) {
                return myDistance(currentObstacle, other) < currentObstacle.getSize() / 2;
            };
            auto closeBoidsBegin = std::find_if(std::begin(boids), std::end(boids), isClose);
            auto closeBoidsEnd   = std::find_if_not(closeBoidsBegin, std::end(boids), isClose);
            for (auto boid = closeBoidsBegin; boid != closeBoidsEnd; ++boid)
            {
                func(*boid);
            }
        };
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
        applyToBoidsInside([&](Boid& other) {
            removeBoidIfNecessary(other);
        });
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

void boidsBehaviorManager(const CharacterCamera& camera, std::vector<Boid>& boids, Parameters characterForce, DiscreteRandomVariable<BoidBehavior>& behaviorVar, const bool spotLight)
{
    for (Boid& boid : boids)
    {
        float distance = myDistance(camera, boid);
        float factor   = 1;
        if (spotLight)
        {
            factor = 3;
        }
        if (distance < characterForce.distance * factor)
        {
            if (boid.getTimeNearCharacter() == 0)
            {
                boid.setBehavior(behaviorVar.generate());
            }
            boid.increaseTimeNearCharacter();
        }
        else
        {
            if (boid.getTimeNearCharacter() != 0)
            {
                boid.resetTimeNearCharacter();
                boid.setBehavior(BoidBehavior::Neutral);
            }
        }
        if (boid.getBehavior() == BoidBehavior::Attacks)
        {
            boid.characterCohesion(camera.getPosition(), characterForce.strength);
        }
        else if (boid.getBehavior() == BoidBehavior::Flees)
        {
            boid.characterCohesion(camera.getPosition(), -characterForce.strength);
        }
    }
}
glm::mat4 getRotationMatrix(glm::vec3 targetVec)
{
    glm::vec3 originalVec = {0, 0, -1};

    glm::vec3 axis  = glm::cross(originalVec, targetVec);
    float     angle = glm::acos(glm::dot(originalVec, targetVec) / (glm::length(originalVec) * glm::length(targetVec)));

    glm::mat4 rotMatrix = glm::mat4(1.0f);
    if (angle != 0 && !glm::isnan(angle))
    {
        rotMatrix = glm::rotate(rotMatrix, angle, glm::normalize(axis));
    }

    return rotMatrix;
}
void boidsFiringManager(std::vector<Laser>& lasers, std::vector<Boid> boids, const LaserParameters parameters, const glm::vec3 characterPosition, BinomialRandomVariable& fireVar, const BinomialRandomParameters& fireParam, NormalRandomVariable& inaccuracyVar)
{
    for (Boid& boid : boids)
    {
        if (static_cast<uint>(fireParam.secondsToShoot * 60) < boid.getTimeNearCharacter() && boid.getBehavior() == BoidBehavior::Attacks)
        {
            if (fireVar.generate() > 0.0)
            {
                glm::vec3 direction           = glm::normalize(characterPosition - boid.getPosition());
                float     inaccuracyFactor    = 0.2;
                glm::vec3 randomizedDirection = glm::vec3(
                    direction.x + (inaccuracyVar.generate()) * inaccuracyFactor,
                    direction.y + inaccuracyVar.generate() * inaccuracyFactor,
                    direction.z + inaccuracyVar.generate() * inaccuracyFactor
                );
                glm::vec3 finalDirection = glm::normalize(randomizedDirection);
                lasers.emplace_back(boid.getPosition() + finalDirection * (1.1f * parameters.range), parameters, finalDirection, getRotationMatrix(finalDirection), glm::vec3(0, 1, 0));
            }
            boid.setBehavior(BoidBehavior::Flees);
        }
    }
}

void characterFiringManager(std::vector<Laser>& lasers, const LaserParameters parameters, const p6::Context& ctx, const CharacterCamera& camera, GeometricRandomVariable& var, LaserDelays& delays)
{
    if (ctx.key_is_pressed(GLFW_KEY_SPACE))
    {
        delays.topLeft.emplace_back(static_cast<int>(var.generate()));
        delays.topRight.emplace_back(static_cast<int>(var.generate()));
        delays.botLeft.emplace_back(static_cast<int>(var.generate()));
        delays.botRight.emplace_back(static_cast<int>(var.generate()));
    }

    for (auto it = delays.topLeft.begin(); it != delays.topLeft.end();)
    {
        if (*it < 1)
        {
            lasers.emplace_back(camera.getTopLCanonPosition(), parameters, camera.getFrontVector(), camera.getRotationMatrix(), glm::vec3(1, 0, 0));
            it = delays.topLeft.erase(it);
        }
        else
        {
            --(*it);
            ++it;
        }
    }

    for (auto it = delays.topRight.begin(); it != delays.topRight.end();)
    {
        if (*it < 1)
        {
            lasers.emplace_back(camera.getTopRCanonPosition(), parameters, camera.getFrontVector(), camera.getRotationMatrix(), glm::vec3(1, 0, 0));
            it = delays.topRight.erase(it); // Supprimer le délai
        }
        else
        {
            --(*it);
            ++it;
        }
    }

    for (auto it = delays.botLeft.begin(); it != delays.botLeft.end();)
    {
        if (*it < 1)
        {
            lasers.emplace_back(camera.getBotLCanonPosition(), parameters, camera.getFrontVector(), camera.getRotationMatrix(), glm::vec3(1, 0, 0));
            it = delays.botLeft.erase(it); // Supprimer le délai
        }
        else
        {
            --(*it);
            ++it;
        }
    }

    for (auto it = delays.botRight.begin(); it != delays.botRight.end();)
    {
        if (*it < 1)
        {
            lasers.emplace_back(camera.getBotRCanonPosition(), parameters, camera.getFrontVector(), camera.getRotationMatrix(), glm::vec3(1, 0, 0));
            it = delays.botRight.erase(it); // Supprimer le délai
        }
        else
        {
            --(*it);
            ++it;
        }
    }
}

void lasersManager(std::vector<Laser>& lasers, const std::vector<Obstacle>& obstacles, std::vector<Boid>& boids, CharacterCamera& camera)
{
    // Boids and character lose a life if in the field of a laser
    for (auto it = lasers.begin(); it != lasers.end();)
    {
        const float laserField = it->getRange();

        // Check for camera
        const float cameraDistance = myDistance(*it, camera);
        if (cameraDistance < laserField)
        {
            camera.hit();
            it = lasers.erase(it);
        }
        else
        {
            // Check for boids
            auto partitionIt = std::partition(boids.begin(), boids.end(), [&it, laserField](const Boid& boid) {
                const float distance = myDistance(*it, boid);
                return distance >= laserField;
            });

            for (auto partitionIt2 = partitionIt; partitionIt2 != boids.end(); ++partitionIt2)
            {
                partitionIt2->hit();
                camera.hitABoid();
            }

            boids.erase(std::remove_if(partitionIt, boids.end(), [](const Boid& boid) {
                            return boid.getLives() < 1;
                        }),
                        boids.end());

            if (partitionIt != boids.end())
            {
                it = lasers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Lasers erased if out of border or inside an obstacle
    lasers.erase(std::remove_if(lasers.begin(), lasers.end(), [&obstacles](const Laser& laser) {
                     return laser.outOfBorders()
                            || std::any_of(obstacles.begin(), obstacles.end(), [&laser](const Obstacle& obstacle) {
                                   return myDistance(laser, obstacle) < obstacle.getSize() / 2;
                               });
                 }),
                 lasers.end());
}

void lasersDisplacement(std::vector<Laser>& lasers, const float deltaTime)
{
    for (Laser& laser : lasers)
    {
        laser.displacement(deltaTime);
    }
}
