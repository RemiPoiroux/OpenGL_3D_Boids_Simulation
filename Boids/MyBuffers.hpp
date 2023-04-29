#pragma once

#include <GL/gl.h>
#include <vector>
#include "glimac/common.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vbos
{
    GLuint backgroundVbo;
    GLuint characterVboHD;
    GLuint characterVboSD;
    GLuint boidVboHD;
    GLuint boidVboSD;
    GLuint obstacleVboHD;
    GLuint obstacleVboSD;
};

struct Ibos
{
    GLuint backgroundIbo;
    GLuint characterIboHD;
    GLuint characterIboSD;
    GLuint boidIboHD;
    GLuint boidIboSD;
    GLuint obstacleIboHD;
    GLuint obstacleIboSD;
};

struct Vaos
{
    GLuint backgroundVao;
    GLuint characterVaoHD;
    GLuint characterVaoSD;
    GLuint boidVaoHD;
    GLuint boidVaoSD;
    GLuint obstacleVaoHD;
    GLuint obstacleVaoSD;
};

struct Textures
{
    GLuint backgroundTex;
    GLuint characterTex;
    GLuint boidTex;
    GLuint obstacleTex;
};

std::vector<glimac::ShapeVertex> cubeVbo();
std::vector<unsigned int>        cubeIbo();