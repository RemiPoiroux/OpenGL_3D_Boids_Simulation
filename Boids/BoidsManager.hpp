#pragma once

#include "Boid.hpp"

struct NeighborsParameters
{
    Parameters alignment;

    Parameters cohesion;

    Parameters separation;
};

std::vector<Boid> createBoids(size_t nb);

void neighborsManager(std::vector<Boid>& boids, NeighborsParameters parameters);

void borderManager(std::vector<Boid>& boids, Parameters parameters);

void boidsDisplacement(std::vector<Boid>& boids);