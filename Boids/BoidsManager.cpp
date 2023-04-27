#include "BoidsManager.hpp"
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
    std::uniform_real_distribution<float> rand(0.f, 1.0f);

    float random = rand(mt);
    float diff   = b - a;
    float r      = random * diff;
    return a + r;
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
    for (size_t i = 0; i < nb; ++i)
    {
        boids.push_back(randomBoid());
    }
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