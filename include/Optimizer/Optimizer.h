#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

#include "Problem/Problem.h"
#include "SolutionBuilder.h"


class Optimizer {
public:
    Optimizer(SolutionBuilder solutionBuilder, Problem& problem, int maxIterations)
        : solutionBuilder(solutionBuilder),
          problem(problem)
    {}

    virtual ~Optimizer() = default; // Virtual destructor
    virtual double optimize() = 0; 
protected:
    Problem& problem;
    SolutionBuilder solutionBuilder;
    int maxIterations;
    std::vector<double> bestSolution;
    std::vector<double> bestFitnesses;
    std::vector<std::vector<double>> solutions;
};

