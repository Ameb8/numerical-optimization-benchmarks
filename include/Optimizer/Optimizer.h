#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

#include "Problem/Problem.h"
#include "SolutionBuilder.h"


class Optimizer {
public:
    Optimizer(SolutionBuilder& solutionBuilder, Problem& problem, int maxIterations)
        : problem(problem),
          solutionBuilder(solutionBuilder),
          maxIterations(maxIterations)
    {}

    virtual ~Optimizer() = default; // Virtual destructor
    virtual double optimize() = 0; 

    // Getters
    double getBestFitness() { return bestFitnesses.back(); }
    std::vector<double>& getBestSolution() { return bestSolution; }
    std::vector<double>& getBestFitnesses() { return bestFitnesses; }
    std::vector<std::vector<double>>& getSolutions() { return solutions; }
    int getMaxIterations() { return maxIterations; }
    Problem& getProblem() { return problem; }
    SolutionBuilder& getSolutionBuilder() { return solutionBuilder; }


protected:
    Problem& problem;
    SolutionBuilder& solutionBuilder;
    int maxIterations;
    std::vector<double> bestSolution;
    std::vector<double> bestFitnesses;
    std::vector<std::vector<double>> solutions;
};

#endif