#pragma once

#include "glm/gtc/matrix_transform.hpp"

struct CameraParameters
{
    float cameraTranslationForce;
    float fastSpeedFactor;
    float cameraRotationForce;
    float mouseRotationForce;
};

class FreeflyCamera
{
private:
    glm::vec3 m_Position;
    float     m_fPhi;
    float     m_fTheta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

private:
    void computeDirectionVectors();

public:
    FreeflyCamera();

    void moveFront(float delta, float deltaTime);
    void moveLeft(float delta, float deltaTime);

    void rotateLeft(float degrees, float deltaTime);
    void rotateUp(float degrees, float deltaTime);

    glm::vec3 getPosition() const;
    float     getTheta() const;
    float     getPhi() const;
    glm::vec3 getFrontVector() const;
    glm::vec3 getUpVector() const;
    glm::vec3 getLeftVector() const;

    glm::mat4 getViewMatrix() const;
};

bool checksTheta(float theta);