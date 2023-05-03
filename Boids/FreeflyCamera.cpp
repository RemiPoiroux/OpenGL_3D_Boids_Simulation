#include "FreeflyCamera.hpp"
#include <cmath>
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

void BordersCheck(glm::vec3& position)
{
    // clang-format off
    if      (position.x > 1)  {position.x = 1;}
    else if (position.x < -1) {position.x = -1;}

    if      (position.y > 1)  {position.y = 1;}
    else if (position.y < -1) {position.y = -1;}

    if      (position.z > 1)  {position.z = 1;}
    else if (position.z < -1) {position.z = -1;}
    // clang-format on
}

FreeflyCamera::FreeflyCamera()
    : m_Position({0, 0, 0.9}), m_fPhi(glm::pi<float>()), m_fTheta(0), m_FrontVector(), m_LeftVector(), m_UpVector()
{
    this->computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    // clang-format off
    this->m_FrontVector = glm::vec3(std::cos(this->m_fTheta) * std::sin(this->m_fPhi), 
                                    std::sin(this->m_fTheta), 
                                    std::cos(this->m_fTheta) * std::cos(this->m_fPhi));
    this->m_LeftVector  = glm::vec3(std::sin(this->m_fPhi + glm::pi<float>() / 2),
                                    0, 
                                    std::cos(this->m_fPhi + glm::pi<float>() / 2));
    this->m_UpVector    = glm::cross(this->m_FrontVector, this->m_LeftVector);
    // clang-format on
}

bool checksTheta(const float theta)
{
    float modulo = glm::mod(theta, 2 * glm::pi<float>());
    return modulo <= glm::half_pi<float>() || modulo >= 3 * glm::half_pi<float>();
}

void FreeflyCamera::moveFront(const float delta, const float deltaTime)
{
    this->m_Position += delta * this->m_FrontVector * deltaTime;
    BordersCheck(this->m_Position);
}
void FreeflyCamera::moveLeft(const float delta, const float deltaTime)
{
    this->m_Position += delta * this->m_LeftVector * deltaTime;
    BordersCheck(this->m_Position);
}

void FreeflyCamera::rotateLeft(const float degrees, const float deltaTime)
{
    if (checksTheta(this->m_fTheta))
    {
        this->m_fPhi += glm::radians(degrees * deltaTime);
    }
    else
    {
        this->m_fPhi -= glm::radians(degrees * deltaTime);
    }
    this->computeDirectionVectors();
}
void FreeflyCamera::rotateUp(const float degrees, const float deltaTime)
{
    this->m_fTheta += glm::radians(degrees * deltaTime);
    this->computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(this->m_Position, this->m_Position + this->m_FrontVector, this->m_UpVector);
}

glm::vec3 FreeflyCamera::getPosition() const
{
    return this->m_Position;
}
float FreeflyCamera::getTheta() const
{
    return this->m_fTheta;
}
float FreeflyCamera::getPhi() const
{
    return this->m_fPhi;
}
glm::vec3 FreeflyCamera::getFrontVector() const
{
    return this->m_FrontVector;
}
glm::vec3 FreeflyCamera::getLeftVector() const
{
    return this->m_LeftVector;
}
glm::vec3 FreeflyCamera::getUpVector() const
{
    return this->m_UpVector;
}