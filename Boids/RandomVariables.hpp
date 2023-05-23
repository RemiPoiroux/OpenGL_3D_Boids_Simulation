#pragma once

#include <string>
#include <vector>
#include "RandomStats.hpp"

class RandomVariable
{
protected:
    float m_parameter;
    float m_expectation;
    float m_variance;

public:
    virtual ~RandomVariable() = default;
    virtual float generate()  = 0;
    float         getParameter() const { return m_parameter; }
    float         getExpectation() const { return m_expectation; }
    float         getVariance() const { return m_variance; }
};

class UniformRandomVariable : public RandomVariable
{
private:
    UnchangedVarStats m_stats;

public:
    explicit UniformRandomVariable(float size);
    float             generate() override;
    UnchangedVarStats getStats() const { return m_stats; };
};

class ExponentialRandomVariable : public RandomVariable
{
private:
    UnchangedVarStats m_stats;

public:
    explicit ExponentialRandomVariable(float expectation);
    float             generate() override;
    UnchangedVarStats getStats() const { return m_stats; };
};

class PoissonRandomVariable : public RandomVariable
{
private:
    UnchangedVarStats m_stats;

public:
    explicit PoissonRandomVariable(float expectation);
    float             generate() override;
    UnchangedVarStats getStats() const { return m_stats; };
};

class BernoulliRandomVariable : public RandomVariable
{
private:
    ModifiableVarStats m_stats;

public:
    explicit BernoulliRandomVariable(float successProbability);
    float              generate() override;
    void               modify(float successProbability);
    ModifiableVarStats getStats() const { return m_stats; };
};
template<typename namesType>
class DiscreteRandomVariable
{
private:
    std::vector<float>                    m_probabilities;
    std::vector<namesType>                m_names;
    ModifiableDiscreteVarStats<namesType> m_stats;

public:
    explicit DiscreteRandomVariable(const std::vector<float>& probabilities, const std::vector<namesType>& names);
    void                                  modify(const std::vector<float>& probabilities);
    namesType                             generate();
    std::vector<float>                    getProbabilities() const { return m_probabilities; }
    std::vector<namesType>                getNames() const { return m_names; }
    ModifiableDiscreteVarStats<namesType> getStats() const { return m_stats; };
};

class BinomialRandomVariable : public RandomVariable
{
private:
    uint               m_trialsNb;
    ModifiableVarStats m_stats;

public:
    explicit BinomialRandomVariable(float successProbability, uint trialsNb);
    void               modify(float successProbability, uint trialsNb);
    float              generate() override;
    uint               getTrialsNb() const { return m_trialsNb; };
    ModifiableVarStats getStats() const { return m_stats; };
};

class NormalRandomVariable : public RandomVariable
{
private:
    float              m_standardDeviation;
    ModifiableVarStats m_stats;

public:
    explicit NormalRandomVariable(float variance);
    void modify(float variance);

    float              generate() override;
    ModifiableVarStats getStats() const { return m_stats; };
};

class GeometricRandomVariable : public RandomVariable
{
private:
    ModifiableVarStats m_stats;

public:
    explicit GeometricRandomVariable(float successProbability);
    void               modify(float successProbability);
    float              generate() override;
    ModifiableVarStats getStats() const { return m_stats; };
};

template class DiscreteRandomVariable<BoidBehavior>;