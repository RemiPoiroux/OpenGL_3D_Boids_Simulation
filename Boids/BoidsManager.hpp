#pragma once

#include "Boid.hpp"

std::vector<Boid> createBoids(const size_t nb);

void neighborsManager(std::vector<Boid>& boids, const float parameters[6]);

void borderManager(std::vector<Boid>& boids, const float distance, const float strength);

void boidsDisplacement(std::vector<Boid>& boids);