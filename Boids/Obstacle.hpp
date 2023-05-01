#pragma once

#include "p6/p6.h"

class Obstacle
{
    glm::vec3 m_position;
    float     m_size;

public:
    Obstacle(glm::vec3 position, float size);
    glm::vec3 pos() const;
    float     size() const;
};

struct ObstaclesParameters
{
    size_t number;
    float  minSize;
    float  maxSize;
    float  strength;
};

float RandomFloat(float a, float b);

glm::vec3 RandomVec3(float maxCoord);

std::vector<Obstacle> createObstacles(ObstaclesParameters p);