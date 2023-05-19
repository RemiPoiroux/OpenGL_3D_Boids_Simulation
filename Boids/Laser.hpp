#pragma once
#include "glm/ext/matrix_float4x4.hpp"
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
    glm::mat4       m_rotationMatrix;
    glm::vec3       m_color;
    int             m_delay;

public:
    Laser(glm::vec3 position, LaserParameters parameters, glm::vec3 direction, glm::mat4 rotationMatrix, glm::vec3 color, int delay);

    glm::vec3 getPosition() const;
    float     getRange() const;
    glm::mat4 getRotationMatrix() const;
    glm::vec3 getColor() const;

    int  getDelay() const;
    void reduceDelay();

    bool outOfBorders() const;

    void displacement(float deltaTime);
};