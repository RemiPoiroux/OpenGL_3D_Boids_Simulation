#pragma once

#include <map>
#include "glimac/common.hpp"
#include "p6/p6.h"

struct MyBuffers
{
    std::map<std::string, GLuint> m_{
        {"background", 0},
        {"character", 0},
        {"characterReactors", 0},
        {"laser", 0},
        {"boidHD", 0},
        {"boidLD", 0},
        {"obstacleHD", 0},
        {"obstacleLD", 0}};
};

void initializesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);

void releasesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);