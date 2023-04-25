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

std::vector<Boid> createBoids(const size_t nb);

void neighborsManager(std::vector<Boid>& boids, const NeighborsParameters parameters);

void borderManager(std::vector<Boid>& boids, const float distance, const float strength);

void boidsDisplacement(std::vector<Boid>& boids);