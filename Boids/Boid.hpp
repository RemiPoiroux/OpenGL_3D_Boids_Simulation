#pragma once

#include "glm/fwd.hpp"
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

    float distance(Boid boid) const;

    void slowing(float amount);

    glm::vec3 TurningDirection(AxisIndex axisIndex) const;
    glm::vec3 HalfTurnDirection(AxisIndex axisIndex) const;

    void ChecksBordersOnAxis(AxisIndex axisIndex, Parameters parameters);

public:
    Boid(glm::vec3 position, float maxSpeed, glm::vec3 direction);

    void acceleration(float a);

    void displacement();

    void neighborsAlignement(const Boid& boid, Parameters parameters);

    void neighborsCohesion(const Boid& boid, Parameters parameters);

    void neighborsSeparation(const Boid& boid, Parameters parameters);

    void bordersAvoidance(Parameters parameters);
};