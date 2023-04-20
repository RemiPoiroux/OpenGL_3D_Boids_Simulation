#pragma once

#include "glm/gtc/matrix_transform.hpp"

class FreeflyCamera
{
    private:
    glm::vec3 m_Position;
    float m_fPhi;
    float m_fTheta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

    private:
    void computeDirectionVectors();

    public:
    FreeflyCamera();

    void moveFront(const float delta);
    void moveLeft(const float delta);

    void rotateLeft(const float degrees);
    void rotateUp(const float degrees);

    glm::mat4 getViewMatrix() const;
};