#include "Laser.hpp"
#include "Boid.hpp"

Laser::Laser(const glm::vec3 position, const LaserParameters parameters, const glm::vec3 direction, const glm::mat4 rotationMatrix, const glm::vec3 color, const int delay)
    : m_position(position), m_parameters(parameters), m_direction(direction), m_rotationMatrix(rotationMatrix), m_color(color), m_delay(delay)
{}

glm::vec3 Laser::getPosition() const
{
    return this->m_position;
}

float Laser::getRange() const
{
    return this->m_parameters.range;
}

glm::mat4 Laser::getRotationMatrix() const
{
    return this->m_rotationMatrix;
}

glm::vec3 Laser::getColor() const
{
    return this->m_color;
}

int Laser::getDelay() const
{
    return m_delay;
}

void Laser::reduceDelay()
{
    --m_delay;
}

bool Laser::outOfBorders() const
{
    // clang-format off
    if  (this->m_position.x > 1)  {return true;}
    if  (this->m_position.x < -1) {return true;}

    if  (this->m_position.y > 1)  {return true;}
    if  (this->m_position.y < -1) {return true;}

    if  (this->m_position.z > 1)  {return true;}
    if  (this->m_position.z < -1) {return true;}

    return false;
    // clang-format on
}

void Laser::displacement(const float deltaTime)
{
    if (getDelay() < 1)
    {
        this->m_position += this->m_direction * m_parameters.speed * deltaTime;
    }
}