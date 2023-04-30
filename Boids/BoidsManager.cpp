#include "BoidsManager.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

/////////////////////////////////
// PARAMETERS

const float ACCELERATION  = 0.01;
const float MAX_SPEED_MIN = 0.0003;
const float MAX_SPEED_MAX = 0.0004;

/////////////////////////////////

float RandomFloat(const float a, const float b)
{
    std::random_device                    rd;
    std::mt19937                          mt(rd());
    std::uniform_real_distribution<float> rand(a, b);
    return rand(mt);
}

glm::vec3 RandomVec3()
{
    glm::vec3 vec(RandomFloat(-1, 1), RandomFloat(-1, 1), RandomFloat(-1, 1));
    return vec;
}

glm::vec3 RandomDirection()
{
    glm::vec3 vec = RandomVec3();
    glm::normalize(vec);
    return vec;
}

Boid randomBoid()
{
    return {RandomVec3(), RandomFloat(MAX_SPEED_MIN, MAX_SPEED_MAX), RandomDirection()};
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
                return currentBoid->distance(other) < distance;
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

void borderManager(std::vector<Boid>& boids, const Parameters parameters)
{
    for (Boid& boid : boids)
    {
        boid.bordersAvoidance(parameters);
    }
}

void boidsDisplacement(std::vector<Boid>& boids)
{
    for (Boid& boid : boids)
    {
        boid.acceleration(ACCELERATION);
        boid.displacement();
    }
}