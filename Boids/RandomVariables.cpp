#include "RandomVariables.hpp"
#include <cmath>
#include <cstdlib>
#include <random>
#include <stdexcept>
#include "RandomStats.hpp"

float randomFloatBtw0and1()
{
    std::random_device                    rd;
    std::mt19937                          mt(rd());
    std::uniform_real_distribution<float> rand(0, 1);
    return rand(mt);
}

UniformRandomVariable::UniformRandomVariable(const float size)
    : RandomVariable()
{
    m_parameter   = size;
    m_expectation = size / 2;
    m_variance    = (size * size) / 12;
    initializeUnchangedVarStats(m_expectation, m_stats, m_variance);
}
float UniformRandomVariable::generate()
{
    float generation = randomFloatBtw0and1() * m_parameter;
    uptateUnchangedVarStats(m_stats, generation);
    return generation;
}

ExponentialRandomVariable::ExponentialRandomVariable(const float expectation)
    : RandomVariable()
{
    m_parameter   = 1 / expectation;
    m_expectation = expectation;
    m_variance    = expectation * expectation;
    initializeUnchangedVarStats(m_expectation, m_stats, m_variance);
}
float ExponentialRandomVariable::generate()
{
    float generation = -std::log(1 - randomFloatBtw0and1()) / m_parameter;
    uptateUnchangedVarStats(m_stats, generation);
    return generation;
}

PoissonRandomVariable::PoissonRandomVariable(const float expectation)
    : RandomVariable()
{
    m_parameter   = expectation;
    m_expectation = expectation;
    m_variance    = expectation;
    initializeUnchangedVarStats(m_expectation, m_stats, m_variance);
}
float PoissonRandomVariable::generate()
{
    float L = std::exp(-m_parameter);
    float k = 0;
    float p = 1;
    while (p > L)
    {
        k++;
        p *= randomFloatBtw0and1();
    }
    float generation = k - 1;
    uptateUnchangedVarStats(m_stats, generation);
    return generation;
}

BernoulliRandomVariable::BernoulliRandomVariable(float successProbability)
    : RandomVariable()
{
    m_parameter   = successProbability;
    m_expectation = successProbability;
    m_variance    = successProbability * (1 - successProbability);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
void BernoulliRandomVariable::modify(float successProbability)
{
    m_parameter   = successProbability;
    m_expectation = successProbability;
    m_variance    = successProbability * (1 - successProbability);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
float BernoulliRandomVariable::generate()
{
    float generation = randomFloatBtw0and1() < m_parameter ? 1 : 0;
    updateModifiableVarStats(m_stats, generation);
    return generation;
}

template<typename T>
DiscreteRandomVariable<T>::DiscreteRandomVariable(const std::vector<float>& probabilities, const std::vector<T>& names)
    : m_probabilities(probabilities), m_names(names)
{
    if (m_probabilities.size() != m_names.size())
    {
        throw std::invalid_argument("The number of probabilities must match the number of values.");
    }

    float sum = 0.0;
    for (float probability : m_probabilities)
    {
        if (probability < 0.0)
        {
            throw std::invalid_argument("Probabilities must be non-negative.");
        }
        sum += probability;
    }
    if (std::abs(sum - 1.0) > 0.0001)
    {
        throw std::invalid_argument("Probabilities must sum to 1.");
    }
    initializationDiscreteVarStats(m_stats, names, probabilities);
}
template<typename T>
void DiscreteRandomVariable<T>::modify(const std::vector<float>& probabilities)
{
    m_probabilities = probabilities;
    if (m_probabilities.size() != m_names.size())
    {
        throw std::invalid_argument("The number of probabilities must match the number of values.");
    }

    float sum = 0.0;
    for (float probability : m_probabilities)
    {
        if (probability < 0.0)
        {
            throw std::invalid_argument("Probabilities must be non-negative.");
        }
        sum += probability;
    }
    if (std::abs(sum - 1.0) > 0.0001)
    {
        throw std::invalid_argument("Probabilities must sum to 1.");
    }
    initializationDiscreteVarStats(m_stats, m_names, m_probabilities);
}
template<typename T>
T DiscreteRandomVariable<T>::generate()
{
    float randomValue           = randomFloatBtw0and1();
    float cumulativeProbability = 0.0;

    for (size_t i = 0; i < m_probabilities.size(); ++i)
    {
        cumulativeProbability += m_probabilities[i];
        if (randomValue <= cumulativeProbability)
        {
            T generation = m_names[i];
            updateDiscreteVarStats(m_stats, generation);
            return generation;
        }
    }
    return T();
}

BinomialRandomVariable::BinomialRandomVariable(const float successProbability, const uint trialsNb)
    : RandomVariable(), m_trialsNb(trialsNb)
{
    m_parameter   = successProbability;
    m_expectation = successProbability * static_cast<float>(trialsNb);
    m_variance    = successProbability * (1 - successProbability) * static_cast<float>(trialsNb);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
void BinomialRandomVariable::modify(const float successProbability, const uint trialsNb)
{
    m_trialsNb    = trialsNb;
    m_parameter   = successProbability;
    m_expectation = successProbability * static_cast<float>(trialsNb);
    m_variance    = successProbability * (1 - successProbability) * static_cast<float>(trialsNb);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
float BinomialRandomVariable::generate()
{
    float result = 0;
    for (uint i = 0; i < m_trialsNb; ++i)
    {
        if (randomFloatBtw0and1() < m_parameter)
            ++result;
    }
    updateModifiableVarStats(m_stats, result);
    return result;
}

NormalRandomVariable::NormalRandomVariable(const float variance)
    : RandomVariable(), m_standardDeviation(std::sqrt(variance))
{
    m_variance    = variance;
    m_parameter   = 0.0f;
    m_expectation = 0.0f;
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
void NormalRandomVariable::modify(float variance)
{
    m_standardDeviation = std::sqrt(variance);
    m_variance          = variance;
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
float NormalRandomVariable::generate()
{
    float u1         = randomFloatBtw0and1();
    float u2         = randomFloatBtw0and1();
    auto  z1         = static_cast<float>(std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2));
    float generation = m_standardDeviation * z1;
    updateModifiableVarStats(m_stats, generation);
    return generation;
}

GeometricRandomVariable::GeometricRandomVariable(float successProbability)
    : RandomVariable()
{
    m_parameter   = successProbability;
    m_expectation = 1 / successProbability;
    m_variance    = (1 - successProbability) / (successProbability * successProbability);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
void GeometricRandomVariable::modify(float successProbability)
{
    m_parameter   = successProbability;
    m_expectation = 1 / successProbability;
    m_variance    = (1 - successProbability) / (successProbability * successProbability);
    initializeModifiableVarStats(m_expectation, m_stats, m_variance);
}
float GeometricRandomVariable::generate()
{
    float p = 1.0f;
    float x = 0.0f;
    while (p > m_parameter)
    {
        p *= randomFloatBtw0and1();
        ++x;
    }
    updateModifiableVarStats(m_stats, x);
    return x;
}