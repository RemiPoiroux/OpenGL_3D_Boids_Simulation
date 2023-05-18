#include "RandomVariables.hpp"
#include <cmath>
#include <cstdlib>

UniformRandomVariable::UniformRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = parameter / 2;
    m_variance = (parameter * parameter) / 12;
}
float UniformRandomVariable::generate() const
{
    return static_cast<float>(std::rand()) / RAND_MAX * m_parameter;
}

ExponentialRandomVariable::ExponentialRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = 1 / parameter;
    m_variance = 1 / (parameter * parameter);
}
float ExponentialRandomVariable::generate() const
{
    return -m_parameter * std::log(1 - static_cast<float>(std::rand()) / RAND_MAX);
}

NormalRandomVariable::NormalRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = parameter;
    m_variance = parameter * parameter;
}
float NormalRandomVariable::generate() const
{
    float u1 = static_cast<float>(std::rand()) / RAND_MAX;
    float u2 = static_cast<float>(std::rand()) / RAND_MAX;
    float z1 = std::sqrt(-2 * std::log(u1)) * std::cos(2 * M_PI * u2);
    return m_parameter + z1;
}

PoissonRandomVariable::PoissonRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = parameter;
    m_variance = parameter;
}
float PoissonRandomVariable::generate() const
{
    float L = std::exp(-m_parameter);
    float k = 0;
    float p = 1;
    do {
        k++;
        p *= static_cast<float>(std::rand()) / RAND_MAX;
    } while (p > L);
    return k - 1;
}

DiscreteRandomVariable::DiscreteRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = parameter;
    m_variance = parameter * (1 - parameter);
}
float DiscreteRandomVariable::generate() const
{
    if (static_cast<float>(std::rand()) / RAND_MAX < m_parameter) {
        return true;
    } else {
        return false;
    }
}

GammaRandomVariable::GammaRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = parameter;
    m_variance = parameter;
}
float GammaRandomVariable::generate() const
{
    float sum = 0;
    int n = static_cast<int>(m_parameter);
    for (int i = 0; i < n; i++) {
        sum += -std::log(static_cast<float>(std::rand()) / RAND_MAX);
    }
    return sum;
}

LogNormalRandomVariable::LogNormalRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = std::exp(parameter + 0.5 * parameter * parameter);
    m_variance = (std::exp(parameter * parameter) - 1) * std::exp(2 * parameter + parameter * parameter);
}
float LogNormalRandomVariable::generate() const
{
    float z = (std::log(1 / (static_cast<float>(std::rand()) / RAND_MAX)) - m_parameter) / m_parameter;
    return std::exp(z);
}

NegativeBinomialRandomVariable::NegativeBinomialRandomVariable(const float parameter)
    : RandomVariable()
{
    m_parameter = parameter;
    m_expectation = (1 - parameter) / parameter;
    m_variance = (1 - parameter) / (parameter * parameter);
}
float NegativeBinomialRandomVariable::generate() const
{
    float r = m_parameter;
    float p = 0.5;
    float x = 0;
    while (static_cast<float>(std::rand()) / RAND_MAX > p) 
    {
        x++;   
        p = r / (r + x);
    }
    return x;
}