#pragma once

class RandomVariable
{
protected:
    float m_parameter;
    float m_expectation;
    float m_variance;

public:
    virtual ~RandomVariable()      = default;
    virtual float generate() const = 0;
    float getParameter() const {return m_parameter;}
    float         getExpectation() const { return m_expectation; }
    float         getVariance() const { return m_variance; }
};

class UniformRandomVariable : public RandomVariable
{
public:
    explicit UniformRandomVariable(float parameter);
    float generate() const override;
};

class ExponentialRandomVariable : public RandomVariable
{
public:
    explicit ExponentialRandomVariable(float parameter);
    float generate() const override;
};

class NormalRandomVariable : public RandomVariable
{
public:
    explicit NormalRandomVariable(float parameter);
    float generate() const override;
};

class PoissonRandomVariable : public RandomVariable
{
public:
    explicit PoissonRandomVariable(float parameter);
    float generate() const override;
};

class DiscreteRandomVariable : public RandomVariable
{
public:
    explicit DiscreteRandomVariable(float parameter);
    float generate() const override;
};

class GammaRandomVariable : public RandomVariable
{
public:
    explicit GammaRandomVariable(float parameter);
    float generate() const override;
};

class LogNormalRandomVariable : public RandomVariable
{
public:
    explicit LogNormalRandomVariable(float parameter);
    float generate() const override;
};

class NegativeBinomialRandomVariable : public RandomVariable
{
public:
    explicit NegativeBinomialRandomVariable(float parameter);
    float generate() const override;
};