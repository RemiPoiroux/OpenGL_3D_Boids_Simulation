#include "glm/fwd.hpp"
#include "p6/p6.h"

#include "Boid.hpp"

/////////////////////////////////
    // PARAMETERS

const float ACCELERATION=0.01;
const float MAX_SPEED_MIN=0.003;
const float MAX_SPEED_MAX=0.004;

/////////////////////////////////

float RandomFloat(const float a, const float b) 
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff   = b - a;
    float r      = random * diff;
    return a + r;
}

glm::vec3 RandomVec3()
{
    glm::vec3 vec(RandomFloat(-1,1),RandomFloat(-1,1),RandomFloat(-1,1));
    return vec;
}

glm::vec3 RandomDirection()
{
    glm::vec3 vec=RandomVec3();
    normaliseVector(vec);
    return vec;
}

Boid randomBoid()
{
    glm::vec3 position = RandomVec3();
    float maxSpeed = RandomFloat(MAX_SPEED_MIN, MAX_SPEED_MAX);
    glm::vec3 direction = RandomDirection();

    return Boid(position, maxSpeed, direction);
}

std::vector<Boid> createBoids(const size_t nb)
{
    std::vector<Boid> boids;
    for(size_t i=0; i<nb; ++i)
    {
        boids.push_back(randomBoid());
    }
    return boids;
}

void neighborsManager(std::vector<Boid>& boids, const float parameters[6])
{
    for(size_t i=0; i<boids.size(); ++i)
    {
        for(size_t j=0; j<boids.size(); ++j)
        {
            if(j!=i)
            {
                boids[i].neighborsAlignement(boids[j], parameters[0], parameters[1]);
                boids[i].neighborsCohesion(boids[j], parameters[2], parameters[3]);
                boids[i].neighborsSeparation(boids[j], parameters[4], parameters[5]);
            }
        }    
    }
}

void borderManager(std::vector<Boid>& boids, const float distance, const float strength)
{
    for(Boid & boid : boids)
    {
        boid.bordersAvoidance(distance, strength);
    }
}

void boidsDisplacement(std::vector<Boid>& boids)
{
    for(Boid & boid : boids)
    {   
        boid.acceleration(ACCELERATION);
        boid.displacement();
    }
}