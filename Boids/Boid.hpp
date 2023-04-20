#pragma once

#include "glm/fwd.hpp"
#include "p6/p6.h"

void normaliseVector(glm::vec3 &v);

class Boid
#include "p6/p6.h"
{
    private :
        glm::vec3 position;
        float maxSpeed;
        float speed;
        glm::vec3 direction;

    private : 
        void applyForce(const glm::vec3 direction, const float strength);

        float distance(const Boid boid) const;

        void slowing(const float amount);

    public :

        Boid(const glm::vec3 p, const float mS, const glm::vec3 d);

        void acceleration(const float a);

        void displacement();

        void neighborsAlignement(const Boid boid, const float distance, const float strength);

        void neighborsCohesion(const Boid boid, const float distance, const float strength);

        void neighborsSeparation(const Boid boid, const float distance, const float strength);

        void bordersAvoidance(const float distance, const float strength);
};