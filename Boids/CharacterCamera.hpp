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

    bool m_turningLeft;
    bool m_turningRight;

    int m_lives;

private:
    void computeDirectionVectors();

public:
    explicit CharacterCamera(uint livesNb);

    void moveFront(float delta, float deltaTime);
    void moveLeft(float delta, float deltaTime);
    void characterRotatingLeft();
    void characterRotatingRight();
    void characterNotRotating();

    void rotateLeft(float degrees, float deltaTime);
    void rotateUp(float degrees, float deltaTime);

    glm::vec3 getPosition() const;
    glm::vec3 getFrontVector() const;
    glm::mat4 getRotationMatrix() const;
    glm::vec3 getTopLReactorPosition() const;
    glm::vec3 getTopRReactorPosition() const;
    glm::vec3 getBotLReactorPosition() const;
    glm::vec3 getBotRReactorPosition() const;

    glm::vec3 getTopLCanonPosition() const;
    glm::vec3 getTopRCanonPosition() const;
    glm::vec3 getBotLCanonPosition() const;
    glm::vec3 getBotRCanonPosition() const;

    glm::mat4 getViewMatrix() const;

    int  getLives() const;
    void hit();
    void hitABoid();
};