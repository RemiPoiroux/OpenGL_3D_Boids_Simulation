#pragma once

#include "p6/p6.h"

class OneTextureProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

    GLint m_Texture;

public:
    OneTextureProgram();
    void  use() const;
    GLint uMVPMatrix() const;
    GLint uMVMatrix() const;
    GLint uNormalMatrix() const;
    GLint uTexture() const;
};

class OneTextureAndLightsProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

    GLint m_Texture;

    GLint m_uKd;
    GLint m_uKs;
    GLint m_uShininess;

    GLint m_uDirLightDir_vs;
    GLint m_uDirLightColor;

    GLint m_uNumPointLights;
    GLint m_uPointLightsPositions;
    GLint m_uPointLightsColors;
    GLint m_uPointLightsIntensities;

    GLint m_uDepthMVP;
    GLint m_uShadowMap;

public:
    OneTextureAndLightsProgram();

    void  use() const;
    GLint uMVPMatrix() const;
    GLint uMVMatrix() const;
    GLint uNormalMatrix() const;
    GLint uTexture() const;
    GLint uKd() const;
    GLint uKs() const;

    GLint uShininess() const;

    GLint uDirLightDir_vs() const;

    GLint uDirLightColor() const;

    GLint uNumPointLights() const;
    GLint uPointLightsPositions() const;
    GLint uPointLightsColors() const;
    GLint uPointLightsIntensities() const;
    GLint uDepthMVP() const;
    GLint uShadowMap() const;
};

class ShadowMapProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;

public:
    ShadowMapProgram();
    void  use() const;
    GLint uMVPMatrix() const;
};

class HaloProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

public:
    HaloProgram();
    void  use() const;
    GLint uMVPMatrix() const;
    GLint uMVMatrix() const;
    GLint uNormalMatrix() const;
};