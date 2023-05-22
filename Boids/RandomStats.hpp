#pragma once

#include <sys/types.h>
#include <vector>

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

void initializeUnchangedVarStats(float expectation, UnchangedVarStats& stats, float variance);
void uptateUnchangedVarStats(UnchangedVarStats& stats, float generation);

void initializeModifiableVarStats(float expectation, ModifiableVarStats& stats, float variance);
void updateModifiableVarStats(ModifiableVarStats& stats, float generation);

template<typename namesType>
void initializationDiscreteVarStats(ModifiableDiscreteVarStats<namesType>& stats, const std::vector<namesType>& names, const std::vector<float>& probabilities);
template<typename namesType>
void updateDiscreteVarStats(ModifiableDiscreteVarStats<namesType>, namesType generation);