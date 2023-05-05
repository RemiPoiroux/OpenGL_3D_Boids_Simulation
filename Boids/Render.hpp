#pragma once

#include "Boid.hpp"
#include "FreeflyCamera.hpp"
#include "MyBuffers.hpp"

void render(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Obstacle>& obstacles, bool lowQuality, MyBuffers& vaos, const FreeflyCamera& camera, MyBuffers& textures, const glm::mat4& ProjMatrix, bool& spotLight);