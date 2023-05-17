#include "Programs.hpp"

OneTextureProgram::OneTextureProgram()
    : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), m_Texture(glGetUniformLocation(m_Program.id(), "uTexture"))
{}
void OneTextureProgram::use() const
{
    m_Program.use();
}
GLint OneTextureProgram::uMVPMatrix() const
{
    return m_uMVPMatrix;
}
GLint OneTextureProgram::uMVMatrix() const
{
    return m_uMVMatrix;
}
GLint OneTextureProgram::uNormalMatrix() const
{
    return m_uNormalMatrix;
}
GLint OneTextureProgram::uTexture() const
{
    return m_Texture;
}

OneTextureAndLightsProgram::OneTextureAndLightsProgram()
    : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3DLight.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), m_Texture(glGetUniformLocation(m_Program.id(), "uTexture")), m_uKd(glGetUniformLocation(m_Program.id(), "uKd")), m_uKs(glGetUniformLocation(m_Program.id(), "uKs")), m_uShininess(glGetUniformLocation(m_Program.id(), "uShininess")), m_uDirLightDir_vs(glGetUniformLocation(m_Program.id(), "uDirLightDir_vs")), m_uDirLightColor(glGetUniformLocation(m_Program.id(), "uDirLightColor")), m_uNumPointLights(glGetUniformLocation(m_Program.id(), "uNumPointLights")), m_uPointLightsPositions(glGetUniformLocation(m_Program.id(), "uPointLightsPositions")), m_uPointLightsColors(glGetUniformLocation(m_Program.id(), "uPointLightsColors")), m_uPointLightsIntensities(glGetUniformLocation(m_Program.id(), "uPointLightsIntensities")), m_uDepthMVP(glGetUniformLocation(m_Program.id(), "uDepthMVP")), m_uShadowMap(glGetUniformLocation(m_Program.id(), "uShadowMap"))
{}
void OneTextureAndLightsProgram::use() const
{
    m_Program.use();
}
GLint OneTextureAndLightsProgram::uMVPMatrix() const
{
    return m_uMVPMatrix;
}
GLint OneTextureAndLightsProgram::uMVMatrix() const
{
    return m_uMVMatrix;
}
GLint OneTextureAndLightsProgram::uNormalMatrix() const
{
    return m_uNormalMatrix;
}
GLint OneTextureAndLightsProgram::uTexture() const
{
    return m_Texture;
}
GLint OneTextureAndLightsProgram::uKd() const
{
    return m_uKd;
}
GLint OneTextureAndLightsProgram::uKs() const
{
    return m_uKs;
}
GLint OneTextureAndLightsProgram::uShininess() const
{
    return m_uShininess;
}
GLint OneTextureAndLightsProgram::uDirLightDir_vs() const
{
    return m_uDirLightDir_vs;
}
GLint OneTextureAndLightsProgram::uDirLightColor() const
{
    return m_uDirLightColor;
}
GLint OneTextureAndLightsProgram::uNumPointLights() const
{
    return m_uNumPointLights;
}
GLint OneTextureAndLightsProgram::uPointLightsPositions() const
{
    return m_uPointLightsPositions;
}
GLint OneTextureAndLightsProgram::uPointLightsColors() const
{
    return m_uPointLightsColors;
}
GLint OneTextureAndLightsProgram::uPointLightsIntensities() const
{
    return m_uPointLightsIntensities;
}
GLint OneTextureAndLightsProgram::uDepthMVP() const
{
    return m_uDepthMVP;
}
GLint OneTextureAndLightsProgram::uShadowMap() const
{
    return m_uShadowMap;
}

ShadowMapProgram::ShadowMapProgram()
    : m_Program{p6::load_shader("shaders/shadowMap.vs.glsl", "shaders/shadowMap.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix"))
{}
void ShadowMapProgram::use() const
{
    m_Program.use();
}
GLint ShadowMapProgram::uMVPMatrix() const
{
    return m_uMVPMatrix;
}

HaloProgram::HaloProgram()
    : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/halo3D.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix"))
{}
void HaloProgram::use() const
{
    m_Program.use();
}
GLint HaloProgram::uMVPMatrix() const
{
    return m_uMVPMatrix;
}
GLint HaloProgram::uMVMatrix() const
{
    return m_uMVMatrix;
}
GLint HaloProgram::uNormalMatrix() const
{
    return m_uNormalMatrix;
}