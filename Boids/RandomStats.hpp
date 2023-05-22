#pragma once

#include <sys/types.h>
#include <vector>
#include "Boid.hpp"

struct UnchangedVarStats
{
    float              expectation;
    float              variance;
    std::vector<float> generations;
};

struct ModifiableVarStats
{
    std::vector<float> expectations;
    std::vector<float> variances;
    std::vector<int>   counts;
    std::vector<float> generationsVariances;
};

struct DiscreteStats
{
    std::vector<float> expectations;
    std::vector<uint>  counts;
};
template<typename namesType>
struct ModifiableDiscreteVarStats
{
    std::vector<namesType>     names;
    std::vector<DiscreteStats> stats;
};
template struct ModifiableDiscreteVarStats<BoidBehavior>;

void initializeUnchangedVarStats(float expectation, UnchangedVarStats& stats, float variance);
void uptateUnchangedVarStats(UnchangedVarStats& stats, float generation);

void initializeModifiableVarStats(float expectation, ModifiableVarStats& stats, float variance);
void updateModifiableVarStats(ModifiableVarStats& stats, float generation);

void initializationDiscreteVarStats(ModifiableDiscreteVarStats<BoidBehavior>& stats, const std::vector<BoidBehavior>& names, const std::vector<float>& probabilities);
void updateDiscreteVarStats(ModifiableDiscreteVarStats<BoidBehavior>& stats, BoidBehavior generation);
