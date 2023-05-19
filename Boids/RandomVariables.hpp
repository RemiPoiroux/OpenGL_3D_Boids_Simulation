#pragma once

#include <string>
#include <vector>
#include "Boid.hpp"

class RandomVariable
{
protected:
    float m_parameter;
    float m_expectation;
    float m_variance;

public:
    virtual ~RandomVariable()      = default;
    virtual float generate() const = 0;
    float         getParameter() const { return m_parameter; }
    float         getExpectation() const { return m_expectation; }
    float         getVariance() const { return m_variance; }
};

class UniformRandomVariable : public RandomVariable
{
public:
    explicit UniformRandomVariable(float size);
    float generate() const override;
};
class ExponentialRandomVariable : public RandomVariable
{
public:
    explicit ExponentialRandomVariable(float expectation);
    float generate() const override;
};
class PoissonRandomVariable : public RandomVariable
{
public:
    explicit PoissonRandomVariable(float expectation);
    float generate() const override;
};

class BernoulliRandomVariable : public RandomVariable
{
public:
    explicit BernoulliRandomVariable(float successProbability);
    float generate() const override;
};
template<typename T>
class DiscreteRandomVariable
{
private:
    std::vector<float> m_probabilities;
    std::vector<T>     m_values;

public:
    explicit DiscreteRandomVariable(const std::vector<float>& probabilities, const std::vector<T>& values);

    T                  generate() const;
    std::vector<float> getProbabilities() const { return m_probabilities; }
    std::vector<T>     getValues() const { return m_values; }
};

class BinomialRandomVariable : public RandomVariable
{
private:
    int m_trialsNb;

public:
    explicit BinomialRandomVariable(float successProbability, int trialsNb);
    float generate() const override;
    int   getTrialsNb() const { return m_trialsNb; };
};

class NormalRandomVariable : public RandomVariable
{
public:
    explicit NormalRandomVariable(float expectation);
    float generate() const override;
};

class GeometricRandomVariable : public RandomVariable
{
public:
    explicit GeometricRandomVariable(float successProbability);
    float generate() const override;
};

template class DiscreteRandomVariable<BoidBehavior>;