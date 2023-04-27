#include "Boid.hpp"
#include <cmath>
#include <cstddef>
#include <iostream>
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
    return static_cast<float>(sqrt(pow(this->position.x - boid.position.x, 2) + pow(this->position.y - boid.position.y, 2) + pow(this->position.z - boid.position.z, 2)));
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

glm::vec3 Boid::TurningDirection(const int axisIndex) const
{
    switch (axisIndex)
    {
    case 0: return {-this->direction.x, this->direction.y, this->direction.z}; // x
    case 1: return {this->direction.x, -this->direction.y, this->direction.z}; // y
    case 2: return {this->direction.x, this->direction.y, -this->direction.z}; // z
    default: return {0, 0, 0};
    }
}

glm::vec3 Boid::HalfTurnDirection(const int axisIndex) const
{
    switch (axisIndex)
    {
    case 0: return {-this->direction.x, 0, 0}; // x
    case 1: return {0, -this->direction.y, 0}; // y
    case 2: return {0, 0, -this->direction.z}; // z
    default: return {0, 0, 0};
    }
}

void Boid::ChecksBordersOnAxis(const int axisIndex, const Parameters p)
{
    if ((this->position[axisIndex] > (1 - p.distance))
        && this->direction[axisIndex] > 0)
    {
        if (this->position[axisIndex] > 1)
        {
            this->applyForce(this->HalfTurnDirection(axisIndex), 2);
        }
        else
        {
            float strength = (p.distance + this->position[axisIndex] - 1) * p.strength;
            this->applyForce(this->TurningDirection(axisIndex), strength);
            this->slowing(strength);
        }
    }
    else if ((this->position[axisIndex] < -(1 - p.distance)) && this->direction[axisIndex] < 0)
    {
        if (this->position[axisIndex] < -1)
        {
            this->applyForce(-this->HalfTurnDirection(axisIndex), 2);
        }
        else
        {
            float strength = (p.distance - this->position[axisIndex] - 1) * p.strength;
            this->applyForce(-this->TurningDirection(axisIndex), strength);
            this->slowing(strength);
        }
    }
}

void Boid::neighborsAlignement(const Boid& boid, const Parameters p)
{
    if (this->distance(boid) < p.distance)
    {
        this->applyForce(boid.direction - this->direction, p.strength);
    }
}
void Boid::neighborsCohesion(const Boid& boid, const Parameters p)
{
    if (this->distance(boid) < p.distance)
    {
        this->applyForce(boid.position - this->position, p.strength);
    }
}
void Boid::neighborsSeparation(const Boid& boid, const Parameters p)
{
    if (this->distance(boid) < p.distance)
    {
        this->applyForce(boid.direction - this->direction, -p.strength);
        this->applyForce(boid.position - this->position, -p.strength / 10);
    }
}

void Boid::bordersAvoidance(const Parameters p)
{
    for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        this->ChecksBordersOnAxis(axisIndex, p); // 0:x axis, 1:y, 2:z
    }
}