#include "Boid.hpp"
#include <cmath>
#include <cstddef>
#include "glm/fwd.hpp"
#include "p6/p6.h"

void normaliseVector(glm::vec3& v)
{
    float norm = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x        = v.x / norm;
    v.y        = v.y / norm;
    v.z        = v.z / norm;
}

void Boid::applyForce(const glm::vec3 direction, const float strength)
{
    this->direction += direction * strength;
    normaliseVector(this->direction);
}

float Boid::distance(const Boid boid) const
{
    return sqrt(pow(this->position.x - boid.position.x, 2) + pow(this->position.y - boid.position.y, 2) + pow(this->position.z - boid.position.z, 2));
}

void Boid::slowing(const float amount)
{
    this->speed -= this->speed * amount;
}

Boid::Boid(const glm::vec3 p, const float mS, const glm::vec3 d)
    : position(p), maxSpeed(mS), speed(), direction(d) {}

void Boid::acceleration(float a)
{
    if (this->speed < this->maxSpeed)
    {
        this->speed += a * this->maxSpeed;
    }
}
void Boid::displacement()
{
    this->position += this->direction * this->speed;
}

// clang-format off
glm::vec3 Boid::TurningDirection(const size_t axisIndex) const 
{
    if(axisIndex==0){return {1, -this->direction.y, -this->direction.z};} // x
    if(axisIndex==1){return {-this->direction.x, 1, -this->direction.z};} // y
    if(axisIndex==2){return {-this->direction.x, -this->direction.y, 1};} // z
}
glm::vec3 Boid::HalfTurnDirection(const size_t axisIndex) const
{
    if(axisIndex==0){return {-1, -this->direction.y, -this->direction.z};} // x
    if(axisIndex==1){return {-this->direction.x, -1, -this->direction.z};} // y
    if(axisIndex==2){return {-this->direction.x, -this->direction.y, -1};} // z
}
// clang-format on

void Boid::ChecksBordersOnAxis(const size_t axisIndex, const float distance, float strength)
{
    if ((this->position[axisIndex] > (1 - distance))
        && this->direction[axisIndex] > 0)
    {
        if (this->position[axisIndex] > 1)
        {
            this->applyForce(this->HalfTurnDirection(axisIndex), 2);
        }
        else
        {
            strength = (distance + this->position[axisIndex] - 1) * strength;
            this->applyForce(this->TurningDirection(axisIndex), strength);
            this->slowing(strength);
        }
    }
    else if ((this->position[axisIndex] < -(1 - distance)) && this->direction[axisIndex] < 0)
    {
        if (this->position[axisIndex] < -1)
        {
            this->applyForce(-this->HalfTurnDirection(axisIndex), 2);
        }
        else
        {
            strength = (distance - this->position[axisIndex] - 1) * strength;
            this->applyForce(-this->TurningDirection(axisIndex), strength);
            this->slowing(strength / 2);
        }
    }
}

void Boid::neighborsAlignement(const Boid boid, const float distance, const float strength)
{
    if (this->distance(boid) < distance)
    {
        this->applyForce(boid.direction - this->direction, strength);
    }
}
void Boid::neighborsCohesion(const Boid boid, const float distance, const float strength)
{
    if (this->distance(boid) < distance)
    {
        this->applyForce(boid.position - this->position, strength);
    }
}
void Boid::neighborsSeparation(const Boid boid, const float distance, const float strength)
{
    if (this->distance(boid) < distance)
    {
        this->applyForce(boid.direction - this->direction, -strength);
        this->applyForce(boid.position - this->position, -strength / 10);
    }
}

void Boid::bordersAvoidance(const float distance, const float strength)
{
    for (size_t axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        this->ChecksBordersOnAxis(axisIndex, distance, strength); // 0:x axis, 1:y, 2:z
    }
}