#pragma once

#include "p6/p6.h"

class ShadowMapFBO
{
private:
    GLuint  m_fbo;
    GLuint  m_shadowMap;
    GLsizei m_width;
    GLsizei m_height;

public:
    ShadowMapFBO(GLsizei width, GLsizei height);

    ~ShadowMapFBO();

    void Init();

    void BindForWriting() const;

    void BindForReading(GLenum TextureUnit) const;
};