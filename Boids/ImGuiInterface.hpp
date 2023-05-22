#pragma once

#include "RandomManager.hpp"
#include "Render.hpp"
#include "SimulationManager.hpp"

void gameImGuiInterface(LodsParameters& lodsParameters, Parameters& obstaclesParameters, NeighborsParameters& neighborsParameters, Parameters& bordersParameters, LaserParameters& lasersParameters, int lives, Parameters& characterForce, int boidsNB, RandomVariablesParameters& randomParameters, RandomVariables& randomVariables);

void endImGuiInterface(bool victory, RandomVariables randomVariables);