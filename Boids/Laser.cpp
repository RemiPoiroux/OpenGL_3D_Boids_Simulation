#include "Laser.hpp"

Laser::Laser(glm::vec3 position, LaserParameters parameters, glm::vec3 direction)
    : m_position(position), m_parameters(parameters), m_direction(direction)
{}

glm::vec3 Laser::getPosition() const
{
    return this->m_position;
}

float Laser::getRange() const
{
    return this->m_parameters.range;
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
    this->m_position += this->m_direction * deltaTime * m_parameters.speed;
}