#include "glm/fwd.hpp"
#include "p6/p6.h"

#include "Boid.hpp"

void normaliseVector(glm::vec3 &v)
{
    float norm = sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
    v.x=v.x/norm;
    v.y=v.y/norm;
    v.z=v.z/norm;
}

void Boid::applyForce(const glm::vec3 direction, const float strength)
{
    this->direction+=direction*strength;
    normaliseVector(this->direction);
}

float Boid::distance(const Boid boid) const
{
    return sqrt(pow(this->position.x-boid.position.x, 2)+pow(this->position.y-boid.position.y, 2)+pow(this->position.z-boid.position.z, 2));
}

void Boid::slowing(const float amount)
{
    this->speed-=this->speed*amount;
}

Boid::Boid(const glm::vec3 p, const float mS, const glm::vec3 d): position(p), maxSpeed(mS), speed(), direction(d) {}

void Boid::acceleration(float a)
{
    if(this->speed<this->maxSpeed)
    {
        this->speed+=a*this->maxSpeed;
    }
}
void Boid::displacement()
{
    this->position+=this->direction*this->speed;
}

void Boid::neighborsAlignement(const Boid boid, const float distance, const float strength)
{
    if(this->distance(boid)<distance)
    {
        this->applyForce(boid.direction-this->direction,strength);
    }
}
void Boid::neighborsCohesion(const Boid boid, const float distance, const float strength)
{
    if(this->distance(boid)<distance)
    {
        this->applyForce(boid.position-this->position,strength);
    }
}
void Boid::neighborsSeparation(const Boid boid, const float distance, const float strength)
{
    if(this->distance(boid)<distance)
    {
        this->applyForce(boid.direction-this->direction,-strength);
        this->applyForce(boid.position-this->position,-strength/10);
    }
}

void Boid::bordersAvoidance(const float distance, float strength)
{
    if((this->position.x>(1-distance)) 
    && this->direction.x>0)
    {
        if(this->position.x>1)
        {
            glm::vec3 direction(-1,-this->direction.y,-this->direction.z);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(-1,this->direction.y,this->direction.z);
            strength=(distance+this->position.x/1-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength);
        }
    }
    else if((this->position.x<-(1-distance))
    && this->direction.x<0)
    {
        if(this->position.x<-1)
        {
            glm::vec3 direction(1,-this->direction.y,-this->direction.z);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(1,this->direction.y,this->direction.z);
            strength=(distance-this->position.x-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength/2);
        }
    }

    if((this->position.y>(1-distance)) 
    && this->direction.y>0)
    {
        if(this->position.y>1)
        {
            glm::vec3 direction(-this->direction.x, -1,-this->direction.z);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(this->direction.x,-1,this->direction.z);
            strength=(distance+this->position.y-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength);
        }
    }
    else if((this->position.y<-(1-distance))
    && this->direction.y<0)
    {
        if(this->position.y<-1)
        {
            glm::vec3 direction(-this->direction.x, 1,-this->direction.z);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(this->direction.x,1,this->direction.z);
            strength=(distance-this->position.y-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength);
        }
    }

    if((this->position.z>(1-distance)) 
    && this->direction.z>0)
    {
        if(this->position.z>1)
        {
            glm::vec3 direction(-this->direction.x,-this->direction.y,-1);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(this->direction.x,this->direction.y,-1);
            strength=(distance+this->position.x-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength);
        }
    }
    else if((this->position.z<-(1-distance))
    && this->direction.z<0)
    {
        if(this->position.z<-1)
        {
            glm::vec3 direction(-this->direction.x,-this->direction.y,1);
            this->applyForce(direction, 2);
        }
        else
        {
            glm::vec3 direction(this->direction.x,this->direction.y,1);
            strength=(distance-this->position.x-1)*strength;
            this->applyForce(direction,strength);
            this->slowing(strength/2);
        }
    }
}