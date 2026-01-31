#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "Optimizer/Optimizer.h"

class LocalSearch : public Optimizer {
private:
    double delta;
    int numNeighbors;

    void localSearch();

public:
    LocalSearch(SolutionBuilder solutionBuilder, Problem& problem, int maxIterations, double delta, int numNeighbors)
        : Optimizer(solutionBuilder, problem, maxIterations),
          delta(delta),
          numNeighbors(numNeighbors)
    {}

    double optimize() override;
};

#endif

