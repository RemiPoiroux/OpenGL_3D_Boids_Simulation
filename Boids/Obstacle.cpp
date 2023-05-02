#include "Obstacle.hpp"

Obstacle::Obstacle(const glm::vec3 position, const float size, const glm::vec3 rotationAxis)
    : m_position(position), m_size(size), m_rotationAxis(rotationAxis) {}

glm::vec3 Obstacle::pos() const
{
    return this->m_position;
}
float Obstacle::size() const
{
    return this->m_size;
}
glm::vec3 Obstacle::rotationAxis() const
{
    return this->m_rotationAxis;
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
    float size = RandomFloat(p.minSize, p.maxSize);
    return {RandomVec3(1 - size), size, RandomVec3(1)};
}

std::vector<Obstacle> createObstacles(ObstaclesParameters p)
{
    std::vector<Obstacle> obstacles;

    auto checkOverlap = [&obstacles](Obstacle obstacle) {
        return std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& o) {
            double distance = sqrt(pow(obstacle.pos().x - o.pos().x, 2) + pow(obstacle.pos().y - o.pos().y, 2));
            return distance < (obstacle.size() + o.size()) / 2;
        });
    };

    for (size_t i = 0; i < p.number; ++i)
    {
        Obstacle obstacle = randomObstacle(p);
        while (checkOverlap(obstacle))
        {
            obstacle = randomObstacle(p);
        }
        obstacles.push_back(obstacle);
    }
    return obstacles;
}