#pragma once

#include "glm/detail/qualifier.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct CameraParameters
{
    float cameraTranslationForce;
    float fastSpeedFactor;
    float cameraRotationForce;
    float mouseRotationForce;
};

class CharacterCamera
{
private:
    glm::vec3 m_Position;
    float     m_fPhi;
    float     m_fTheta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    bool m_firing;
    bool m_turningLeft;
    bool m_turningRight;

private:
    void computeDirectionVectors();

public:
    CharacterCamera();

    void moveFront(float delta, float deltaTime);
    void moveLeft(float delta, float deltaTime);

    void rotateLeft(float degrees, float deltaTime);
    void rotateUp(float degrees, float deltaTime);

    glm::vec3 pos() const;
    glm::vec3 getFrontVector() const;
    glm::mat4 getRotationMatrix() const;
    glm::vec3 getTopLReactorPosition() const;
    glm::vec3 getTopRReactorPosition() const;
    glm::vec3 getBotLReactorPosition() const;
    glm::vec3 getBotRReactorPosition() const;

    bool isFiring() const;

    glm::mat4 getViewMatrix() const;
};