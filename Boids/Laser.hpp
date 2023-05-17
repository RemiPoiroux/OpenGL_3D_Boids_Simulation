#pragma once
#include "glm/ext/vector_float3.hpp"

struct LaserParameters
{
    float speed;
    float range;
};

class Laser
{
private:
    glm::vec3       m_position;
    LaserParameters m_parameters;
    glm::vec3       m_direction;

public:
    Laser(glm::vec3 position, LaserParameters parameters, glm::vec3 direction);

    glm::vec3 getPosition() const;
    float     getRange() const;

    bool outOfBorders() const;

    void displacement(float deltaTime);
};