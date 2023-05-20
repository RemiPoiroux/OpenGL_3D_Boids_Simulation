#include "CharacterCamera.hpp"
#include <cmath>
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_float4x4.hpp"
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

CharacterCamera::CharacterCamera(const uint livesNb)
    : m_Position({0, 0, 0.9}), m_fPhi(glm::pi<float>()), m_fTheta(), m_FrontVector(), m_LeftVector(), m_UpVector(), m_turningLeft(false), m_turningRight(false), m_lives(static_cast<int>(livesNb))
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

void CharacterCamera::characterRotatingLeft()
{
    m_turningLeft = true;
}
void CharacterCamera::characterRotatingRight()
{
    m_turningRight = true;
}
void CharacterCamera::characterNotRotating()
{
    m_turningLeft  = false;
    m_turningRight = false;
}

glm::mat4 CharacterCamera::getViewMatrix() const
{
    return glm::lookAt(this->m_Position, this->m_Position + this->m_FrontVector, this->m_UpVector);
}

glm::vec3 CharacterCamera::getPosition() const
{
    return this->m_Position + 0.08f * this->m_FrontVector - 0.02f * this->m_UpVector;
}
glm::vec3 CharacterCamera::getFrontVector() const
{
    return this->m_FrontVector;
}
glm::mat4 CharacterCamera::getRotationMatrix() const
{
    glm::mat4 rotationMatrix = glm::mat4(1);
    if (this->m_turningLeft)
    {
        rotationMatrix = glm::rotate(rotationMatrix, -.3f, this->m_FrontVector);
    }
    if (this->m_turningRight)
    {
        rotationMatrix = glm::rotate(rotationMatrix, .3f, this->m_FrontVector);
    }
    rotationMatrix = glm::rotate(rotationMatrix, atan2f(this->m_FrontVector.x, this->m_FrontVector.z), {0, 1, 0});
    rotationMatrix = glm::rotate(rotationMatrix, -glm::asin(this->m_FrontVector.y), {1, 0, 0});
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (!checksTheta(this->m_fTheta))
    {
        rotationMatrix = glm::scale(rotationMatrix, {-1, -1, 1});
    }
    return rotationMatrix;
}

glm::vec3 reactorsOffset()
{
    return {0.01, 0.004, -0.04};
}
glm::vec3 CharacterCamera::getTopLReactorPosition() const
{
    return this->getPosition() + reactorsOffset().x * this->m_LeftVector + reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getTopRReactorPosition() const
{
    return this->getPosition() - reactorsOffset().x * this->m_LeftVector + reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getBotLReactorPosition() const
{
    return this->getPosition() + reactorsOffset().x * this->m_LeftVector - reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}
glm::vec3 CharacterCamera::getBotRReactorPosition() const
{
    return this->getPosition() - reactorsOffset().x * this->m_LeftVector - reactorsOffset().y * this->m_UpVector + reactorsOffset().z * this->m_FrontVector;
}

glm::vec3 canonsOffset()
{
    return {0.043f, 0.006f, 0.07f};
}

glm::mat4 canonRotationMatrix(bool turningLeft, bool turningRight, glm::vec3 frontVector)
{
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    if (turningLeft)
    {
        rotationMatrix = glm::rotate(rotationMatrix, -0.3f, frontVector);
    }
    if (turningRight)
    {
        rotationMatrix = glm::rotate(rotationMatrix, 0.3f, frontVector);
    }

    return rotationMatrix;
}

glm::vec3 rotatedLeftVector(const glm::mat4& rotationMatrix, const glm::vec3& leftVector)
{
    glm::vec3 rotatedLeftVector = glm::vec3(rotationMatrix * glm::vec4(leftVector, 0.0f));
    return rotatedLeftVector;
}

glm::vec3 rotatedUpVector(const glm::mat4& rotationMatrix, const glm::vec3& upVector)
{
    glm::vec3 rotatedUpVector = glm::vec3(rotationMatrix * glm::vec4(upVector, 0.0f));
    return rotatedUpVector;
}

glm::vec3 CharacterCamera::getTopLCanonPosition() const
{
    glm::vec3 offset         = canonsOffset();
    glm::mat4 rotationMatrix = canonRotationMatrix(m_turningLeft, m_turningRight, this->m_FrontVector);
    glm::vec3 rotatedLeft    = rotatedLeftVector(rotationMatrix, m_LeftVector);
    glm::vec3 rotatedUp      = rotatedUpVector(rotationMatrix, m_UpVector);

    glm::vec3 rotatedPosition = getPosition() + offset.x * rotatedLeft + offset.y * rotatedUp + offset.z * this->m_FrontVector;
    return rotatedPosition;
}

glm::vec3 CharacterCamera::getTopRCanonPosition() const
{
    glm::vec3 offset         = canonsOffset();
    glm::mat4 rotationMatrix = canonRotationMatrix(m_turningLeft, m_turningRight, this->m_FrontVector);
    glm::vec3 rotatedLeft    = rotatedLeftVector(rotationMatrix, m_LeftVector);
    glm::vec3 rotatedUp      = rotatedUpVector(rotationMatrix, m_UpVector);

    glm::vec3 rotatedPosition = getPosition() - offset.x * rotatedLeft + offset.y * rotatedUp + offset.z * this->m_FrontVector;
    return rotatedPosition;
}

glm::vec3 CharacterCamera::getBotLCanonPosition() const
{
    glm::vec3 offset         = canonsOffset();
    glm::mat4 rotationMatrix = canonRotationMatrix(m_turningLeft, m_turningRight, this->m_FrontVector);
    glm::vec3 rotatedLeft    = rotatedLeftVector(rotationMatrix, m_LeftVector);
    glm::vec3 rotatedUp      = rotatedUpVector(rotationMatrix, m_UpVector);

    glm::vec3 rotatedPosition = getPosition() + offset.x * rotatedLeft - offset.y * rotatedUp + offset.z * this->m_FrontVector;
    return rotatedPosition;
}

glm::vec3 CharacterCamera::getBotRCanonPosition() const
{
    glm::vec3 offset         = canonsOffset();
    glm::mat4 rotationMatrix = canonRotationMatrix(m_turningLeft, m_turningRight, this->m_FrontVector);
    glm::vec3 rotatedLeft    = rotatedLeftVector(rotationMatrix, m_LeftVector);
    glm::vec3 rotatedUp      = rotatedUpVector(rotationMatrix, m_UpVector);

    glm::vec3 rotatedPosition = getPosition() - offset.x * rotatedLeft - offset.y * rotatedUp + offset.z * this->m_FrontVector;
    return rotatedPosition;
}

int CharacterCamera::getLives() const
{
    return m_lives;
}

void CharacterCamera::hit()
{
    --m_lives;
}

void CharacterCamera::hitABoid()
{
    ++m_lives;
}