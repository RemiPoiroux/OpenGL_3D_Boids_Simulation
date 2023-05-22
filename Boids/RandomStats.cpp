#include "RandomStats.hpp"
#include <algorithm>

void initializeUnchangedVarStats(float expectation, UnchangedVarStats& stats, float variance)
{
    stats.expectation = expectation;
    stats.variance    = variance;
}

void uptateUnchangedVarStats(UnchangedVarStats& stats, float generation)
{
    stats.generations.emplace_back(generation);
}

void initializeModifiableVarStats(const float expectation, ModifiableVarStats& stats, const float variance)
{
    stats.expectations.emplace_back(expectation);
    stats.variances.emplace_back(variance);
    stats.counts.emplace_back(0);
    stats.generationsVariances.emplace_back(0);
}
float newVariance(const float newValue, const float expectation, float previousVariance, const int previousCount)
{
    auto previousCountFloat = static_cast<float>(previousCount);

    float newCount = previousCountFloat + 1;

    float newSumOfSquares = (previousVariance + expectation * expectation) * previousCountFloat + newValue * newValue;

    float newSum = expectation * previousCountFloat + newValue;

    float newExpectation = newSum / newCount;

    float newVariance = (newSumOfSquares / newCount) - (newExpectation * newExpectation);

    return newVariance;
}
void updateModifiableVarStats(ModifiableVarStats& stats, const float generation)
{
    stats.generationsVariances.back() = newVariance(generation, stats.expectations.back(), stats.generationsVariances.back(), stats.counts.back());
    ++stats.counts.back();
}

template<typename namesType>
void initializationDiscreteVarStats(ModifiableDiscreteVarStats<namesType>& stats, const std::vector<namesType>& names, const std::vector<float>& probabilities)
{
    if (!names.empty())
    {
        stats.names = names;
    }
    DiscreteStats discreteStats;
    discreteStats.expectations = probabilities;
    discreteStats.counts.reserve(probabilities.size());

    stats.stats.emplace_back(discreteStats);
}

template<typename namesType>
void updateDiscreteVarStats(ModifiableDiscreteVarStats<namesType>& stats, const namesType& generation)
{
    auto it = std::find(stats.names.begin(), stats.names.end(), generation);
    if (it != stats.names.end())
    {
        size_t index = std::distance(stats.names.begin(), it);

        ++stats.stats[index].counts;
    }
}