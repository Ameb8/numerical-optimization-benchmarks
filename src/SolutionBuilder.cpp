#include "SolutionBuilder.h"


std::vector<double> SolutionBuilder::getRand() {
    std::vector<double> solution(dimensions);

    // Generate random solutions
    for(int i = 0; i < dimensions; i++) {{
            solution[i] = lower + (upper - lower) * mt.genrand_real1(); 
        }
    }
    
    return solution;
}

std::vector<std::vector<double>> SolutionBuilder::getNeighbors(
    const std::vector<double>& center,
    int numNeighbors,
    int delta
) {
    std::vector<std::vector<double>> solutions(dimensions, std::vector<double>());

    // Randomly generate neighbors within maxDelta for each dimension
    for(int i = 0; i < numNeighbors; i++) {
        for(int j = 0; j < dimensions; j++) {
            // Increment randomly within maxDelta range
            double delta = (2.0 * mt.genrand_real1() - 1) * maxDelta;
            solutions[i][j] = center[j] + delta;
        }
    }

    
    return solutions;
}
