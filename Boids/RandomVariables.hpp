#pragma once

#include <string>
#include <vector>

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
    explicit UniformRandomVariable(float limits);
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

class DiscreteRandomVariable
{
private:
    std::vector<float>       m_probabilities;
    std::vector<std::string> m_names;

public:
    explicit DiscreteRandomVariable(const std::vector<float>& probabilities, const std::vector<std::string>& names);
    std::string              generate() const;
    std::vector<float>       getProbabilities() const { return m_probabilities; }
    std::vector<std::string> getNames() const { return m_names; }
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