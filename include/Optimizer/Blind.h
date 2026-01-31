#ifndef BLIND_H
#define BLIND_H

#include "Optimizer/Optimizer.h"

class Blind : public Optimizer {
public:
    Blind(SolutionBuilder solutionBuilder, Problem& problem, int maxIterations, double delta, int numNeighbors)
        : Optimizer(solutionBuilder, problem, maxIterations)
    {
        bestFitnesses.resize(maxIterations);
        bestSolution.resize(solutionBuilder.getDimensions());
        solutions.resize(maxIterations);
    }

    double optimize() override;
};
