#include "BoidsManager.hpp"
#include <algorithm>
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

/////////////////////////////////
// PARAMETERS

const float ACCELERATION  = 0.01;
const float MAX_SPEED_MIN = 0.003;
const float MAX_SPEED_MAX = 0.004;

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
    normaliseVector(vec);
    return vec;
}

Boid randomBoid()
{
    glm::vec3 position  = RandomVec3();
    float     maxSpeed  = RandomFloat(MAX_SPEED_MIN, MAX_SPEED_MAX);
    glm::vec3 direction = RandomDirection();

    return {position, maxSpeed, direction};
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
    for (Boid& boid : boids)
    {
        auto isNotSelf = [&](const Boid& other) {
            return &boid != &other;
        };
        auto getAlignment = [&](const Boid& other) {
            return boid.neighborsAlignement(other, parameters.alignment);
        };
        auto getCohesion = [&](const Boid& other) {
            return boid.neighborsCohesion(other, parameters.cohesion);
        };
        auto getSeparation = [&](const Boid& other) {
            return boid.neighborsSeparation(other, parameters.separation);
        };

        std::vector<Boid> neighbors(boids.size() - 1);
        std::copy_if(boids.begin(), boids.end(), neighbors.begin(), isNotSelf);
        std::transform(neighbors.begin(), neighbors.end(), neighbors.begin(), getAlignment);
        std::transform(neighbors.begin(), neighbors.end(), neighbors.begin(), getCohesion);
        std::transform(neighbors.begin(), neighbors.end(), neighbors.begin(), getSeparation);
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