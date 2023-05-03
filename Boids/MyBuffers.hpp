#pragma once

#include <map>
#include <vector>
#include "Boid.hpp"
#include "FreeflyCamera.hpp"
#include "glimac/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct MyBuffers
{
    std::map<std::string, GLuint> m_{
        {"background", 0},
        {"character", 0},
        {"characterReactors", 0},
        {"boidHD", 0},
        {"boidLD", 0},
        {"obstacleHD", 0},
        {"obstacleLD", 0}};
};

void initializesBuffers(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);

void render(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Obstacle>& obstacles, bool lowQuality, MyBuffers& vaos, const FreeflyCamera& camera, MyBuffers& textures, const glm::mat4& ProjMatrix);

void releasesRessources(MyBuffers& vbos, MyBuffers& ibos, MyBuffers& vaos, MyBuffers& textures);