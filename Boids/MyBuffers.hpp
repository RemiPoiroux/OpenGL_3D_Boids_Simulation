#pragma once

#include <map>
#include <vector>
#include "glimac/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct MyBuffers
{
    std::map<std::string, GLuint> m_{
        {"background", 0},
        {"characterHD", 0},
        {"characterSD", 0},
        {"boidHD", 0},
        {"boidSD", 0},
        {"obstacleHD", 0},
        {"obstacleSD", 0}};
};

std::vector<glimac::ShapeVertex> cubeVbo();
std::vector<unsigned int>        cubeIbo();

void initializesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);

void releasesRessources(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);