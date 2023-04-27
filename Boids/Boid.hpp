#pragma once

#include "glm/fwd.hpp"
#include "p6/p6.h"

struct Parameters
{
    float distance;
    float strength;
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

    glm::vec3 TurningDirection(int axisIndex) const;
    glm::vec3 HalfTurnDirection(int axisIndex) const;

    void ChecksBordersOnAxis(int axisIndex, Parameters parameters);

public:
    Boid(glm::vec3 p, float mS, glm::vec3 d);

    void acceleration(float a);

    void displacement();

    void neighborsAlignement(const Boid& boid, Parameters parameters);

    void neighborsCohesion(const Boid& boid, Parameters parameters);

    void neighborsSeparation(const Boid& boid, Parameters parameters);

    void bordersAvoidance(Parameters parameters);
};