#pragma once

#include "glm/fwd.hpp"
#include "p6/p6.h"

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

    glm::vec3 TurningDirection(size_t axisIndex) const;
    glm::vec3 HalfTurnDirection(size_t axisIndex) const;

    void ChecksBordersOnAxis(size_t axisIndex, float distance, float strength);

public:
    Boid(glm::vec3 p, float mS, glm::vec3 d);

    void acceleration(float a);

    void displacement();

    void neighborsAlignement(Boid boid, float distance, float strength);

    void neighborsCohesion(Boid boid, float distance, float strength);

    void neighborsSeparation(Boid boid, float distance, float strength);

    void bordersAvoidance(float distance, float strength);
};