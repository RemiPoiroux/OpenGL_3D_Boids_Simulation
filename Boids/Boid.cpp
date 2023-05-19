#include "Boid.hpp"
#include "Obstacle.hpp"
#include "glm/fwd.hpp"
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

glm::vec3 Boid::getPosition() const
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

Boid::Boid(const glm::vec3 pos, const float maxSpeed, const glm::vec3 dir, int lives)
    : position(pos), maxSpeed(maxSpeed), speed(), direction(dir), lives(lives), timeNearCharacter(), timeBeforeFiring(), behavior(BoidBehavior::Neutral)
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

glm::vec3 Boid::halfTurnDirection(const AxisIndex axisIndex) const
{
    switch (axisIndex)
    {
    case AxisIndex::x: return {-this->direction.x, 0, 0};
    case AxisIndex::y: return {0, -this->direction.y, 0};
    case AxisIndex::z: return {0, 0, -this->direction.z};
    }
    return {};
}

void Boid::checksBordersOnAxis(const AxisIndex axisIndex, const Parameters p)
{
    if ((this->position[axisIndex] > (1 - p.distance))
        && this->direction[axisIndex] > 0)
    {
        if (this->position[axisIndex] > 1)
        {
            this->applyForce(this->halfTurnDirection(axisIndex), 2);
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
            this->applyForce(this->halfTurnDirection(axisIndex), 2);
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

void Boid::characterCohesion(glm::vec3 characterPosition, float strength)
{
    this->applyForce(characterPosition - this->position, strength);
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
    this->applyForce(boid.position - this->position, -strength);
}

void Boid::obstacleAvoidance(const Obstacle& obstacle, const float strength)
{
    glm::vec3 newDirection = glm::normalize((obstacle.getPosition() - this->position));
    if (strength < 1)
    {
        auto random_orthogonal_vector = [](glm::vec3 v) {
            std::random_device               rd;
            std::mt19937                     gen(rd());
            std::uniform_real_distribution<> dis(-1.0, 1.0);
            glm::vec3                        rand_vec(dis(gen), dis(gen), dis(gen));

            glm::vec3 proj = rand_vec - v * (dot(rand_vec, v) / glm::length(v));

            return glm::normalize(proj);
        };
        newDirection += (random_orthogonal_vector(newDirection) / 3.f);
    }
    this->applyForce(newDirection, -strength);
}

void Boid::bordersAvoidance(const Parameters p)
{
    this->checksBordersOnAxis(AxisIndex::x, p);
    this->checksBordersOnAxis(AxisIndex::y, p);
    this->checksBordersOnAxis(AxisIndex::z, p);
}

int Boid::getLives() const
{
    return lives;
}
void Boid::hit()
{
    --lives;
}

uint Boid::getTimeBeforeFiring() const
{
    return timeBeforeFiring;
}
void Boid::setTimeBeforeFiring(const uint time)
{
    timeBeforeFiring = time;
}
uint Boid::getTimeNearCharacter() const
{
    return timeNearCharacter;
}
void Boid::increaseTimeNearCharacter()
{
    ++timeNearCharacter;
}
void Boid::resetTimeNearCharacter()
{
    timeNearCharacter = 0;
}

BoidBehavior Boid::getBehavior() const
{
    return behavior;
}
void Boid::setBehavior(BoidBehavior newBehavior)
{
    behavior = newBehavior;
}