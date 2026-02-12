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

double SolutionBuilder::checkBounds(double value) {
    if(value > upper)
        value = upper;
    else if(value < lower)
        value = lower;

    return value;
}

std::vector<std::vector<double>> SolutionBuilder::getNeighbors(
    const std::vector<double>& center,
    int numNeighbors,
    double maxDelta
) {
    std::vector<std::vector<double>> solutions(numNeighbors, std::vector<double>(dimensions));

    // Randomly generate neighbors within maxDelta for each dimension
    for(int i = 0; i < numNeighbors; i++) {
        for(int j = 0; j < dimensions; j++) {
            // Increment randomly within maxDelta range
            double delta = (2.0 * mt.genrand_real1() - 1) * maxDelta;
            solutions[i][j] = checkBounds(center[j] + delta);
        }
    }

    
    return solutions;
}

std::vector<std::vector<double>> SolutionBuilder::getSubset(
    const std::vector<std::vector<double>>& population,
    int subsetSize,
    int source
) {
    std::vector<std::vector<double>> subset(subsetSize);

    // Create index array 
    std::vector<int> indices(population.size() - 1);
    int idx = 0;
    for (int i = 0; i < population.size(); ++i) {
        if (i != source) {
            indices[idx] = i;
            ++idx;
        }
    }
        

    // Partial Fisher-Yates shuffle
    for (int i = 0; i < subsetSize; ++i) {
        int j = i + (mt.genrand_int32() % (indices.size() - i));
        std::swap(indices[i], indices[j]);
    }

    for(int i = 0; i < subsetSize; ++i)
        subset[i] = population[indices[i]];

    return subset;
}
