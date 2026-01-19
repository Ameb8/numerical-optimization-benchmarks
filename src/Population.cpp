#include "Population.h"
#include <iostream>
#include "External/mt.h"

#define ERROR 0
#define SUCCESS 1

Population::Population(int populationSize, int dimension)
    : n(populationSize), m(dimension),
      solutions(n, std::vector<double>(m, 0.0))
{}

int Population::initialize(double lower, double upper, int seed) {
    if(n < 1 || m < 1) // Validate population size
        return ERROR;

    // Initialize seeded pseudo-random generator
    MersenneTwister mt;
    mt.init_genrand(seed);

    // Generate random solutions
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            solutions[i][j] = lower + (upper - lower) * mt.genrand_real1(); 
        }
    }
    
    return SUCCESS;
}

std::vector<double> Population::evaluate(const Problem& problem) {
    std::vector<double> fitness(n);

    for(int i = 0; i < n; i++) {
        fitness[i] = problem.evaluate(solutions[i]);
    }

    return fitness;
}

const std::vector<std::vector<double>>& Population::getSolutions() const {
    return solutions;
}
