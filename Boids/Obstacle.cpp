#include "Obstacle.hpp"

Obstacle::Obstacle(const glm::vec3 position, const float size)
    : m_position(position), m_size(size) {}

glm::vec3 Obstacle::pos() const
{
    return this->m_position;
}
float Obstacle::size() const
{
    return this->m_size;
}

float RandomFloat(const float a, const float b)
{
    std::random_device                    rd;
    std::mt19937                          mt(rd());
    std::uniform_real_distribution<float> rand(a, b);
    return rand(mt);
}
glm::vec3 RandomVec3(float maxCoord)
{
    glm::vec3 vec(RandomFloat(-maxCoord, maxCoord), RandomFloat(-maxCoord, maxCoord), RandomFloat(-maxCoord, maxCoord));
    return vec;
}

Obstacle randomObstacle(ObstaclesParameters p)
{
    return {RandomVec3(1 - p.maxSize), RandomFloat(p.minSize, p.maxSize)};
}

std::vector<Obstacle> createObstacles(ObstaclesParameters p)
{
    std::vector<Obstacle> obstacles;
    for (size_t i = 0; i < p.number; ++i)
    {
        obstacles.push_back(randomObstacle(p));
    }
    return obstacles;
}