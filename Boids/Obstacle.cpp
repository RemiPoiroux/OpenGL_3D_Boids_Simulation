#include "Obstacle.hpp"
#include "RandomManager.hpp"
#include "RandomVariables.hpp"

Obstacle::Obstacle(const glm::vec3 position, const float size, const glm::vec3 rotationAxis)
    : m_position(position), m_size(size), m_rotationAxis(rotationAxis) {}

glm::vec3 Obstacle::getPosition() const
{
    return this->m_position;
}
float Obstacle::getSize() const
{
    return this->m_size;
}
glm::vec3 Obstacle::getRotationAxis() const
{
    return this->m_rotationAxis;
}

float RandomFloat(const float a, const float b)
{
    float                 size       = b - a;
    UniformRandomVariable uniformVar = UniformRandomVariable(size);
    return a + uniformVar.generate();
}
glm::vec3 RandomVec3(float maxCoord)
{
    glm::vec3 vec(RandomFloat(-maxCoord, maxCoord), RandomFloat(-maxCoord, maxCoord), RandomFloat(-maxCoord, maxCoord));
    return vec;
}

Obstacle randomObstacle(ObstaclesParameters p, ExponentialRandomVariable& expVar)
{
    float size = expVar.generate();
    if (size > p.maxSize)
    {
        size = p.maxSize;
    }
    if (size < p.minSize)
    {
        size = p.minSize;
    }
    return {RandomVec3(1 - size), size, RandomVec3(1)};
}

std::vector<Obstacle> createObstacles(ObstaclesParameters p, ExponentialRandomVariable& expVar)
{
    std::vector<Obstacle> obstacles;
    obstacles.reserve(p.number);

    auto checkOverlap = [&obstacles](Obstacle obstacle) {
        return std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& o) {
            double squaredDistance = pow(obstacle.getPosition().x - o.getPosition().x, 2) + pow(obstacle.getPosition().y - o.getPosition().y, 2);
            return squaredDistance < pow((obstacle.getSize() + o.getSize()) / 2, 2);
        });
    };

    for (size_t i = 0; i < p.number; ++i)
    {
        Obstacle obstacle = randomObstacle(p, expVar);
        while (checkOverlap(obstacle))
        {
            obstacle = randomObstacle(p, expVar);
        }
        obstacles.push_back(obstacle);
    }
    return obstacles;
}