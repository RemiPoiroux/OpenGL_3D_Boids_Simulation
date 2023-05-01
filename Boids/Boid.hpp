#pragma once

#include "p6/p6.h"

struct Parameters
{
    float distance;
    float strength;
};

enum AxisIndex {
    x = 0,
    y = 1,
    z = 2
};

void normaliseVector(glm::vec3& v);

class Boid
{
private:
    glm::vec3 position;
    float     maxSpeed;
    float     speed;
    glm::vec3 direction;

private:
    void applyForce(glm::vec3 direction, float strength);

    void slowing();

    glm::vec3 HalfTurnDirection(AxisIndex axisIndex) const;

    void ChecksBordersOnAxis(AxisIndex axisIndex, Parameters parameters);

public:
    Boid(glm::vec3 position, float maxSpeed, glm::vec3 direction);

    glm::vec3 pos() const;

    glm::mat4 getRotationMatrix() const;

    void acceleration();

    void displacement(float deltaTime);

    void neighborsAlignement(const Boid& boid, float strength);

    void neighborsCohesion(const Boid& boid, float strength);

    void neighborsSeparation(const Boid& boid, float strength);

    void bordersAvoidance(Parameters parameters);
};