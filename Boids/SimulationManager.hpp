#pragma once

#include "Boid.hpp"
#include "CharacterCamera.hpp"
#include "Laser.hpp"
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

void firingManager(std::vector<Laser>& lasers, LaserParameters parameters, const p6::Context& ctx, const CharacterCamera& camera);
void lasersManager(std::vector<Laser>& lasers, const std::vector<Obstacle>& obstacles, std::vector<Boid>& boids);

void borderManager(std::vector<Boid>& boids, Parameters parameters);

void boidsDisplacement(std::vector<Boid>& boids, float deltaTime);

void lasersDisplacement(std::vector<Laser>& lasers, float deltaTime);