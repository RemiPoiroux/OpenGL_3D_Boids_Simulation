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
        for (size_t j = 0; j < boids.size(); ++j)
        {
            if (j != i)
            {
                boids[i].neighborsAlignement(boids[j], parameters.alignment);
                boids[i].neighborsCohesion(boids[j], parameters.cohesion);
                boids[i].neighborsSeparation(boids[j], parameters.separation);
            }
        }
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