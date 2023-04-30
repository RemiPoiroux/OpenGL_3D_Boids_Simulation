#include "Boid.hpp"
#include <cmath>
#include <cstddef>
#include <iostream>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "p6/p6.h"

void Boid::applyForce(const glm::vec3 direction, const float strength)
{
    this->direction += direction * strength;
    glm::normalize(this->direction);
}

float Boid::distance(const Boid boid) const
{
    return static_cast<float>(sqrt(pow(this->position.x - boid.position.x, 2) + pow(this->position.y - boid.position.y, 2) + pow(this->position.z - boid.position.z, 2)));
}
glm::vec3 Boid::pos() const
{
    return position;
}

glm::mat4 Boid::getRotationMatrix() const
{
    glm::vec3 originalVec = {0, 0, -1};
    glm::vec3 targetVec   = this->direction;

    glm::vec3 axis  = glm::cross(originalVec, targetVec);
    float     angle = glm::acos(glm::dot(originalVec, targetVec) / (glm::length(originalVec) * glm::length(targetVec)));

    glm::mat4 rotMatrix = glm::mat4(1.0f);
    if (angle != 0 && !glm::isnan(angle))
    {
        rotMatrix = glm::rotate(rotMatrix, angle, glm::normalize(axis));
    }

    return rotMatrix;
}

void Boid::slowing(const float amount)
{
    this->speed -= this->speed * amount;
}

Boid::Boid(const glm::vec3 pos, const float maxSpeed, const glm::vec3 dir)
    : position(pos), maxSpeed(maxSpeed), speed(), direction(dir)
{}

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
    return {};
}

glm::vec3 Boid::HalfTurnDirection(const AxisIndex axisIndex) const
{
    switch (axisIndex)
    {
    case AxisIndex::x: return {-this->direction.x, 0, 0};
    case AxisIndex::y: return {0, -this->direction.y, 0};
    case AxisIndex::z: return {0, 0, -this->direction.z};
    }
    return {};
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
        // else
        // {
        //     float strength = (p.distance + this->position[axisIndex] - 1) * p.strength;
        //     this->applyForce(this->TurningDirection(axisIndex), strength);
        //     this->slowing(strength / 4);
        // }
    }
    else if ((this->position[axisIndex] < -(1 - p.distance)) && this->direction[axisIndex] < 0)
    {
        if (this->position[axisIndex] < -1)
        {
            this->applyForce(this->HalfTurnDirection(axisIndex), 2);
        }
        // else
        // {
        //     float strength = (p.distance - this->position[axisIndex] - 1) * p.strength;
        //     this->applyForce(this->TurningDirection(axisIndex), strength);
        //     this->slowing(strength / 4);
        // }
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