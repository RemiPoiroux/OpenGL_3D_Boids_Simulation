#pragma once

#include "p6/p6.h"

class BoidProgram
{
private:
    const p6::Shader m_Program;

    const GLint m_uMVPMatrix;
    const GLint m_uMVMatrix;
    const GLint m_uNormalMatrix;

public:
    BoidProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix"))
    {}
    void  use() const { m_Program.use(); }
    GLint uMVPMatrix() const { return m_uMVPMatrix; }
    GLint uMVMatrix() const { return m_uMVMatrix; }
    GLint uNormalMatrix() const { return m_uNormalMatrix; }
};

class CharacterProgram
{
private:
    const p6::Shader m_Program;

    const GLint m_uMVPMatrix;
    const GLint m_uMVMatrix;
    const GLint m_uNormalMatrix;

public:
    CharacterProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix"))
    {}
    void  use() const { m_Program.use(); }
    GLint uMVPMatrix() const { return m_uMVPMatrix; }
    GLint uMVMatrix() const { return m_uMVMatrix; }
    GLint uNormalMatrix() const { return m_uNormalMatrix; }
};

class BackgroundProgram
{
private:
    const p6::Shader m_Program;

    const GLint m_uMVPMatrix;
    const GLint m_uMVMatrix;
    const GLint m_uNormalMatrix;

    const GLint m_starsTexture;

public:
    BackgroundProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}, m_uMVPMatrix(glGetUniformLocation(m_Program.id(), "uMVPMatrix")), m_uMVMatrix(glGetUniformLocation(m_Program.id(), "uMVMatrix")), m_uNormalMatrix(glGetUniformLocation(m_Program.id(), "uNormalMatrix")), m_starsTexture(glGetUniformLocation(m_Program.id(), "uTexture"))
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
        return m_starsTexture;
    }
};