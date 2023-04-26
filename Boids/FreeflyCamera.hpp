#pragma once

#include "glm/gtc/matrix_transform.hpp"

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

    void moveFront(float delta);
    void moveLeft(float delta);

    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};