#pragma once

struct UniformStats
{
};

struct ExponentialStats
{
};

struct PoissonStats
{
};

struct BernoulliStats
{
};

struct DiscreteStats
{
};

struct BinomialStats
{
};

struct NormalStats
{
};

struct GeometricStats
{
};

struct RandomVariablesStats
{
    UniformStats     uniform;
    ExponentialStats exponential;
    PoissonStats     poisson;
    BernoulliStats   bernouilli;
    DiscreteStats    discrete;
    BinomialStats    binomial;
    NormalStats      normal;
    GeometricStats   geometric;
};