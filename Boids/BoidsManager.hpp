#pragma once

#include "Boid.hpp"

struct NeighborsParameters
{
    float AlignmentDistance;
    float AlignementStrength;

    float CohesionDistance;
    float CohesionStrength;

    float SeparationDistance;
    float SeparationStength;
};

std::vector<Boid> createBoids(size_t nb);

void neighborsManager(std::vector<Boid>& boids, NeighborsParameters parameters);

void borderManager(std::vector<Boid>& boids, float distance, float strength);

void boidsDisplacement(std::vector<Boid>& boids);