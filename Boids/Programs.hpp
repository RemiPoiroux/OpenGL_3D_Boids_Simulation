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
    OneTextureProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), m_Texture(glGetUniformLocation(m_Program.id(), "uTexture"))
    {}
    void use() const
    {
        m_Program.use();
    }
    GLint uMVPMatrix() const
    {
        return m_uMVPMatrix;
    }
    GLint uMVMatrix() const
    {
        return m_uMVMatrix;
    }
    GLint uNormalMatrix() const
    {
        return m_uNormalMatrix;
    }
    GLint uTexture() const
    {
        return m_Texture;
    }
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
    OneTextureAndLightsProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3DLight.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), m_Texture(glGetUniformLocation(m_Program.id(), "uTexture")), m_uKd(glGetUniformLocation(m_Program.id(), "uKd")), m_uKs(glGetUniformLocation(m_Program.id(), "uKs")), m_uShininess(glGetUniformLocation(m_Program.id(), "uShininess")), m_uDirLightDir_vs(glGetUniformLocation(m_Program.id(), "uDirLightDir_vs")), m_uDirLightColor(glGetUniformLocation(m_Program.id(), "uDirLightColor")), m_uNumPointLights(glGetUniformLocation(m_Program.id(), "uNumPointLights")), m_uPointLightsPositions(glGetUniformLocation(m_Program.id(), "uPointLightsPositions")), m_uPointLightsColors(glGetUniformLocation(m_Program.id(), "uPointLightsColors")), m_uPointLightsIntensities(glGetUniformLocation(m_Program.id(), "uPointLightsIntensities")), m_uDepthMVP(glGetUniformLocation(m_Program.id(), "uDepthMVP")), m_uShadowMap(glGetUniformLocation(m_Program.id(), "uShadowMap"))
    {}

    void use() const
    {
        m_Program.use();
    }
    GLint uMVPMatrix() const
    {
        return m_uMVPMatrix;
    }
    GLint uMVMatrix() const
    {
        return m_uMVMatrix;
    }
    GLint uNormalMatrix() const
    {
        return m_uNormalMatrix;
    }
    GLint uTexture() const
    {
        return m_Texture;
    }
    GLint uKd() const
    {
        return m_uKd;
    }

    GLint uKs() const
    {
        return m_uKs;
    }

    GLint uShininess() const
    {
        return m_uShininess;
    }

    GLint uDirLightDir_vs() const
    {
        return m_uDirLightDir_vs;
    }

    GLint uDirLightColor() const
    {
        return m_uDirLightColor;
    }

    GLint uNumPointLights() const
    {
        return m_uNumPointLights;
    }

    GLint uPointLightsPositions() const
    {
        return m_uPointLightsPositions;
    }
    GLint uPointLightsColors() const
    {
        return m_uPointLightsColors;
    }
    GLint uPointLightsIntensities() const
    {
        return m_uPointLightsIntensities;
    }
    GLint uDepthMVP() const
    {
        return m_uDepthMVP;
    }
    GLint uShadowMap() const
    {
        return m_uShadowMap;
    }
};

class ShadowMapProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;

public:
    ShadowMapProgram()
        : m_Program{p6::load_shader("shaders/shadowMap.vs.glsl", "shaders/shadowMap.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix"))
    {}
    void use() const
    {
        m_Program.use();
    }
    GLint uMVPMatrix() const
    {
        return m_uMVPMatrix;
    }
};

class HaloProgram
{
private:
    p6::Shader m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

public:
    HaloProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/halo3D.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix"))
    {}
    void use() const
    {
        m_Program.use();
    }
    GLint uMVPMatrix() const
    {
        return m_uMVPMatrix;
    }
    GLint uMVMatrix() const
    {
        return m_uMVMatrix;
    }
    GLint uNormalMatrix() const
    {
        return m_uNormalMatrix;
    }
};