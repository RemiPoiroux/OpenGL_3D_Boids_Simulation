#pragma once

#include "CharacterCamera.hpp"
#include "Obstacle.hpp"

enum AxisIndex {
    x = 0,
    y = 1,
    z = 2
};

enum class BoidBehavior {
    Attacks,
    Neutral,
    Flees
};

void normaliseVector(glm::vec3& v);

class Boid
{
private:
    glm::vec3 position;
    float     maxSpeed;
    float     speed;
    glm::vec3 direction;
    int       lives;

    uint         timeNearCharacter;
    uint         timeBeforeFiring;
    BoidBehavior behavior;

private:
    void applyForce(glm::vec3 direction, float strength);

    void slowing();

    glm::vec3 halfTurnDirection(AxisIndex axisIndex) const;

    void checksBordersOnAxis(AxisIndex axisIndex, Parameters parameters);

public:
    Boid(glm::vec3 position, float maxSpeed, glm::vec3 direction, int lives);

    glm::vec3 getPosition() const;

    glm::mat4 getRotationMatrix() const;

    void acceleration();

    void displacement(float deltaTime);

    void characterCohesion(glm::vec3 position, float strength);

    void neighborsAlignement(const Boid& boid, float strength);

    void neighborsCohesion(const Boid& boid, float strength);

    void neighborsSeparation(const Boid& boid, float strength);

    void obstacleAvoidance(const Obstacle& obstacle, float strength);

    void bordersAvoidance(Parameters parameters);

    int  getLives() const;
    void hit();

    uint getTimeBeforeFiring() const;
    void setTimeBeforeFiring(uint time);
    uint getTimeNearCharacter() const;
    void increaseTimeNearCharacter();
    void resetTimeNearCharacter();

    BoidBehavior getBehavior() const;
    void         setBehavior(BoidBehavior newBehavior);
};