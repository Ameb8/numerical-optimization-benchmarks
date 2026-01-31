#ifndef SOLUTION_BUILDER_H
#define SOLUTION_BUILDER_H

#include "Problem/Problem.h"
#include <vector>
#include "External/mt.h"

class SolutionBuilder { 
private:
    const int dimensions;
    const int lower;
    const int upper;
    MersenneTwister solutionGen;
    MersenneTwister neighborGen;
public:
    // Constructors
    SolutionBuilder(int dimensions, int lower, int upper, int seed)
        : dimensions(dimensions),
          lower(lower),
          upper(upper),
    {
        solutionGen.init_genrand(seed);
    }

    std::vector<double> getRand();

    std::vector<double> getNeighbors(
        const std::vector<double>& center,
        int numNeighbors,
        double delta,
        double lower,
        double upper,
        int seed
    );
};

#endif

    MersenneTwister mt;
    mt.init_genrand(seed);