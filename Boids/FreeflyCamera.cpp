#include "FreeflyCamera.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

void BordersCheck(glm::vec3 &position)
{  
    if(position.x>1){position.x=1;}
    else if(position.x<-1){position.x=-1;}

    if(position.y>1){position.y=1;}
    else if(position.y<-1){position.y=-1;}

    if(position.z>1){position.z=1;}
    else if(position.z<-1){position.z=-1;}
}

FreeflyCamera::FreeflyCamera():m_Position(0),m_fPhi(glm::pi<float>()),m_fTheta(0)
{
    this->computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    this->m_FrontVector=glm::vec3(cos(this->m_fTheta)*sin(this->m_fPhi), sin(this->m_fTheta),cos(this->m_fTheta)*cos(this->m_fPhi));
    this->m_LeftVector=glm::vec3(sin(this->m_fPhi+glm::pi<float>()/2), 0, cos(this->m_fPhi+glm::pi<float>()/2));
    this->m_UpVector=glm::cross(this->m_FrontVector,this->m_LeftVector);
}

void FreeflyCamera::moveFront(const float delta)
{
    this->m_Position+=delta*this->m_FrontVector;
    BordersCheck(this->m_Position);
}
void FreeflyCamera::moveLeft(const float delta)
{
    this->m_Position+=delta*this->m_LeftVector;
    BordersCheck(this->m_Position);
}

void FreeflyCamera::rotateLeft(const float degrees)
{
    this->m_fPhi+=glm::radians(degrees);
    this->computeDirectionVectors();
}
void FreeflyCamera::rotateUp(const float degrees)
{
    this->m_fTheta+=glm::radians(degrees);
    this->computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(this->m_Position, this->m_Position+this->m_FrontVector, this->m_UpVector);
}
