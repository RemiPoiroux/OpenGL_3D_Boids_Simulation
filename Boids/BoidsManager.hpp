#pragma once

#include "Boid.hpp"
#include "Obstacle.hpp"

struct NeighborsParameters
{
    Parameters alignment;

    Parameters cohesion;

    Parameters separation;
};

template<typename T1, typename T2>
float distance(const T1& entity1, const T2& entity2);

std::vector<Boid> createBoids(size_t nb);

void neighborsManager(std::vector<Boid>& boids, NeighborsParameters parameters);

void obstaclesManager(std::vector<Boid>& boids, const std::vector<Obstacle>& obstacles, Parameters parameters);

void borderManager(std::vector<Boid>& boids, Parameters parameters);

void boidsDisplacement(std::vector<Boid>& boids, float deltaTime);