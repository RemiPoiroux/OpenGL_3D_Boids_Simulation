#pragma once

#include "Boid.hpp"
#include "CharacterCamera.hpp"
#include "Laser.hpp"
#include "MyBuffers.hpp"

void render(p6::Context& ctx, std::vector<Boid>& boids, std::vector<Obstacle>& obstacles, std::vector<Laser>& lasers, bool lowQuality, MyBuffers& vaos, const CharacterCamera& camera, MyBuffers& textures, const glm::mat4& ProjMatrix, bool& spotLight);