#pragma once

#include "p6/p6.h"

class OneTextureProgram
{
private:
    const p6::Shader m_Program;

    const GLint m_uMVPMatrix;
    const GLint m_uMVMatrix;
    const GLint m_uNormalMatrix;

    const GLint m_Texture;

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

class HaloProgram
{
private:
    const p6::Shader m_Program;

    const GLint m_uMVPMatrix;
    const GLint m_uMVMatrix;
    const GLint m_uNormalMatrix;

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