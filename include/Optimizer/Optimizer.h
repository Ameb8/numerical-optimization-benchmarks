#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

#include "Problem/Problem.h"
#include "SolutionBuilder.h"


class Optimizer {
public:
    Optimizer(SolutionBuilder solutionBuilder, Problem& problem, int maxIterations)
        : problem(problem),
          solutionBuilder(solutionBuilder),
          maxIterations(maxIterations)
    {}

    virtual ~Optimizer() = default; // Virtual destructor
    virtual double optimize() = 0; 

    // Getters
    double getBestFitness() const { return bestFitnesses.back(); }
    const std::vector<double>& getBestSolution() const { return bestSolution; }
    const std::vector<double>& getBestFitnesses() const { return bestFitnesses; }
    const std::vector<std::vector<double>>& getSolutions() const { return solutions; }
    int getMaxIterations() const { return maxIterations; }
    const Problem& getProblem() const { return problem; }
    const SolutionBuilder& getSolutionBuilder() const { return solutionBuilder; }


protected:
    Problem& problem;
    SolutionBuilder solutionBuilder;
    int maxIterations;
    std::vector<double> bestSolution;
    std::vector<double> bestFitnesses;
    std::vector<std::vector<double>> solutions;
};

