#include "RandomVariables.hpp"
#include <cmath>
#include <cstdlib>
#include <random>
#include <stdexcept>

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
}
float UniformRandomVariable::generate() const
{
    return randomFloatBtw0and1() * m_parameter;
}

ExponentialRandomVariable::ExponentialRandomVariable(const float expectation)
    : RandomVariable()
{
    m_parameter   = 1 / expectation;
    m_expectation = expectation;
    m_variance    = expectation * expectation;
}
float ExponentialRandomVariable::generate() const
{
    return -m_parameter * std::log(1 - randomFloatBtw0and1());
}

PoissonRandomVariable::PoissonRandomVariable(const float expectation)
    : RandomVariable()
{
    m_parameter   = expectation;
    m_expectation = expectation;
    m_variance    = expectation;
}
float PoissonRandomVariable::generate() const
{
    float L = std::exp(-m_parameter);
    float k = 0;
    float p = 1;
    while (p > L)
    {
        k++;
        p *= randomFloatBtw0and1();
    }
    return k - 1;
}

BernoulliRandomVariable::BernoulliRandomVariable(float successProbability)
    : RandomVariable()
{
    m_parameter   = successProbability;
    m_expectation = successProbability;
    m_variance    = successProbability * (1 - successProbability);
}
float BernoulliRandomVariable::generate() const
{
    return randomFloatBtw0and1() < m_parameter ? 1 : 0;
}

template<typename T>
DiscreteRandomVariable<T>::DiscreteRandomVariable(const std::vector<float>& probabilities, const std::vector<T>& values)
    : m_probabilities(probabilities), m_values(values)
{
    if (m_probabilities.size() != m_values.size())
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
}
template<typename T>
T DiscreteRandomVariable<T>::generate() const
{
    float randomValue           = randomFloatBtw0and1();
    float cumulativeProbability = 0.0;

    for (size_t i = 0; i < m_probabilities.size(); ++i)
    {
        cumulativeProbability += m_probabilities[i];
        if (randomValue <= cumulativeProbability)
        {
            return m_values[i];
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
}
float BinomialRandomVariable::generate() const
{
    float result = 0;
    for (uint i = 0; i < m_trialsNb; ++i)
    {
        if (randomFloatBtw0and1() < m_parameter)
            ++result;
    }
    return result;
}

NormalRandomVariable::NormalRandomVariable(const float expectation)
    : RandomVariable()
{
    m_parameter   = expectation;
    m_expectation = expectation;
    m_variance    = expectation * expectation;
}
float NormalRandomVariable::generate() const
{
    float u1 = randomFloatBtw0and1();
    float u2 = randomFloatBtw0and1();
    auto  z1 = static_cast<float>(std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2));
    return m_parameter + z1;
}

GeometricRandomVariable::GeometricRandomVariable(float successProbability)
    : RandomVariable()
{
    m_parameter   = successProbability;
    m_expectation = 1 / successProbability;
    m_variance    = (1 - successProbability) / (successProbability * successProbability);
}
float GeometricRandomVariable::generate() const
{
    float p = 1.0f;
    float x = 0.0f;
    while (p > m_parameter)
    {
        p *= randomFloatBtw0and1();
        ++x;
    }
    return x;
}