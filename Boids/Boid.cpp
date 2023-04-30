#include "Boid.hpp"
#include "glm/gtx/rotate_vector.hpp"

const float ACCELERATION = 0.01f;

float strength(const glm::vec3 boidsPosition, const float distance, const glm::vec3 obstaclePosition)
{
    float d = glm::distance(boidsPosition, obstaclePosition);

    if (d >= distance)
    {
        return 0.f;
    }
    if (d == 0.0f)
    {
        return 1.0f;
    }

    float a = 4.0f / (distance * distance);
    float b = -2.0f / distance;
    float c = 1.0f + b * distance;
    return a * d * d + b * d + c;
}

void normaliseVector(glm::vec3& v)
{
    float norm = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x        = v.x / norm;
    v.y        = v.y / norm;
    v.z        = v.z / norm;
}
// clang-format off
void bordersCheck(glm::vec3& position)
{
    if      (position.x > 1)  {position.x = 1;}
    else if (position.x < -1) {position.x = -1;}

    if      (position.y > 1)  {position.y = 1;}
    else if (position.y < -1) {position.y = -1;}

    if      (position.z > 1)  {position.z = 1;}
    else if (position.z < -1) {position.z = -1;}
}
void speedCheck(float& speed, float& maxSpeed)
{
    if (speed < 0)          {speed = 0;}
    if (speed > maxSpeed)   {speed = maxSpeed;}
}
// clang-format on

void Boid::applyForce(const glm::vec3 direction, const float strength)
{
    this->direction += direction * strength;
    normaliseVector(this->direction);
    this->slowing();
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

Boid::Boid(const glm::vec3 pos, const float maxSpeed, const glm::vec3 dir)
    : position(pos), maxSpeed(maxSpeed), speed(), direction(dir)
{}

void Boid::slowing()
{
    this->speed += 2 * ACCELERATION * this->maxSpeed;
}
void Boid::acceleration()
{
    this->speed += ACCELERATION * this->maxSpeed;
}

void Boid::displacement(const float deltaTime)
{
    bordersCheck(this->position);
    speedCheck(this->speed, this->maxSpeed);
    this->position += this->direction * this->speed * deltaTime;
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
        else
        {
            glm::vec3 obstaclePosition  = this->position;
            obstaclePosition[axisIndex] = 1;
            float strength              = ::strength(this->position, p.distance, obstaclePosition) * p.strength;
            this->applyForce(-this->position, strength);
        }
    }
    else if ((this->position[axisIndex] < -(1 - p.distance)) && this->direction[axisIndex] < 0)
    {
        if (this->position[axisIndex] < -1)
        {
            this->applyForce(this->HalfTurnDirection(axisIndex), 2);
        }
        else
        {
            glm::vec3 obstaclePosition  = this->position;
            obstaclePosition[axisIndex] = -1;
            float strength              = ::strength(this->position, p.distance, obstaclePosition) * p.strength;
            this->applyForce(-this->position, strength);
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