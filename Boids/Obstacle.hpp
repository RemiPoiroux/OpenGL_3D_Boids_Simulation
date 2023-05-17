#pragma once

#include "p6/p6.h"

class Obstacle
{
    glm::vec3 m_position;
    float     m_size;
    glm::vec3 m_rotationAxis;

public:
    Obstacle(glm::vec3 position, float size, glm::vec3 rotationAxis);
    glm::vec3 getPosition() const;
    float     getSize() const;
    glm::vec3 getRotationAxis() const;
};

struct Parameters
{
    float distance;
    float strength;
};

struct ObstaclesParameters
{
    size_t     number;
    float      minSize;
    float      maxSize;
    Parameters force;
};

float RandomFloat(float a, float b);

glm::vec3 RandomVec3(float maxCoord);

std::vector<Obstacle> createObstacles(ObstaclesParameters p);