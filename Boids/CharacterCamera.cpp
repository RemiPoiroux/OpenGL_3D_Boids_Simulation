#include "CharacterCamera.hpp"
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

CharacterCamera::CharacterCamera()
    : m_Position({0, 0, 0.9}), m_fPhi(glm::pi<float>()), m_fTheta(0), m_FrontVector(), m_LeftVector(), m_UpVector(), m_firing(false), m_turningLeft(false), m_turningRight(false)
{
    this->computeDirectionVectors();
}

void CharacterCamera::computeDirectionVectors()
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

void CharacterCamera::moveFront(const float delta, const float deltaTime)
{
    this->m_Position += delta * this->m_FrontVector * deltaTime;
    BordersCheck(this->m_Position);
}
void CharacterCamera::moveLeft(const float delta, const float deltaTime)
{
    this->m_Position += delta * this->m_LeftVector * deltaTime;
    BordersCheck(this->m_Position);
}

void CharacterCamera::rotateLeft(const float degrees, const float deltaTime)
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
void CharacterCamera::rotateUp(const float degrees, const float deltaTime)
{
    this->m_fTheta += glm::radians(degrees * deltaTime);
    this->computeDirectionVectors();
}

glm::mat4 CharacterCamera::getViewMatrix() const
{
    return glm::lookAt(this->m_Position, this->m_Position + this->m_FrontVector, this->m_UpVector);
}

glm::vec3 CharacterCamera::pos() const
{
    return this->m_Position + 0.08f * this->m_FrontVector - 0.02f * this->m_UpVector;
}
glm::vec3 CharacterCamera::getFrontVector() const
{
    return this->m_FrontVector;
}
glm::mat4 CharacterCamera::getRotationMatrix() const
{
    glm::mat4 MVMatrix = glm::rotate(glm::mat4(1), atan2f(this->m_FrontVector.x, this->m_FrontVector.z), {0, 1, 0});
    MVMatrix           = glm::rotate(MVMatrix, -glm::asin(this->m_FrontVector.y), {1, 0, 0});
    MVMatrix           = glm::rotate(MVMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (!checksTheta(this->m_fTheta))
    {
        MVMatrix = glm::scale(MVMatrix, {-1, -1, 1});
    }
    return MVMatrix;
}

glm::vec3 reactorsOffset()
{
    return {0.01, 0.004, -0.04};
}
glm::vec3 CharacterCamera::getTopLReactorPosition() const
{
    return this->pos() + reactorsOffset().x * this->m_LeftVector + reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getTopRReactorPosition() const
{
    return this->pos() - reactorsOffset().x * this->m_LeftVector + reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getBotLReactorPosition() const
{
    return this->pos() + reactorsOffset().x * this->m_LeftVector - reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getBotRReactorPosition() const
{
    return this->pos() - reactorsOffset().x * this->m_LeftVector - reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}

// bool CharacterCamera::isFiring() const;