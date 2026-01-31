#ifndef SOLUTION_BUILDER_H
#define SOLUTION_BUILDER_H

#include <vector>

#include "Problem/Problem.h"
#include "External/mt.h"

class SolutionBuilder { 
private:
    const int dimensions;
    const int lower;
    const int upper;
    MersenneTwister mt;
public:
    // Constructors
    SolutionBuilder(int dimensions, int lower, int upper, int seed)
        : dimensions(dimensions),
          lower(lower),
          upper(upper),
    {
        mt.init_genrand(seed);
    }

    std::vector<double> getRand();

    std::vector<std::vector<double>> getNeighbors(
        const std::vector<double>& center,
        int numNeighbors,
        double delta
    );

    double getDimensions() { return dimensions; }
};

#endif
