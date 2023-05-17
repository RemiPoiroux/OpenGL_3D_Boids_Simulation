#include "Laser.hpp"

Laser::Laser(glm::vec3 position, LaserParameters parameters, glm::vec3 direction, glm::mat4 rotationMatrix)
    : m_position(position), m_parameters(parameters), m_direction(direction), m_rotationMatrix(rotationMatrix)
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
    this->m_position += this->m_direction * m_parameters.speed * deltaTime;
}