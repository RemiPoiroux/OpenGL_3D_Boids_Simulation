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

Boid::Boid(const glm::vec3 pos, const float maxSpeed, const glm::vec3 dir)
    : position(pos), maxSpeed(maxSpeed), speed(), direction(dir) {}

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

glm::vec3 Boid::TurningDirection(const AxisIndex axisIndex) const
{
    switch (axisIndex)
    {
    case AxisIndex::x: return {-this->direction.x, this->direction.y, this->direction.z};
    case AxisIndex::y: return {this->direction.x, -this->direction.y, this->direction.z};
    case AxisIndex::z: return {this->direction.x, this->direction.y, -this->direction.z};
    }
}

glm::vec3 Boid::HalfTurnDirection(const AxisIndex axisIndex) const
{
    switch (axisIndex)
    {
    case AxisIndex::x: return {-this->direction.x, 0, 0};
    case AxisIndex::y: return {0, -this->direction.y, 0};
    case AxisIndex::z: return {0, 0, -this->direction.z};
    }
}

void Boid::ChecksBordersOnAxis(const AxisIndex axisIndex, const Parameters p)
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

void Boid::neighborsAlignement(const Boid& boid, const float strength)
{
    this->applyForce(boid.direction - this->direction, strength);
}
void Boid::neighborsCohesion(const Boid& boid, const float strength)
{
    this->applyForce(boid.position - this->position, strength);
}
void Boid::neighborsSeparation(const Boid& boid, const float strength)
{
    this->applyForce(boid.direction - this->direction, -strength);
    this->applyForce(boid.position - this->position, -strength / 10);
}

void Boid::bordersAvoidance(const Parameters p)
{
    this->ChecksBordersOnAxis(AxisIndex::x, p);
    this->ChecksBordersOnAxis(AxisIndex::y, p);
    this->ChecksBordersOnAxis(AxisIndex::z, p);
}