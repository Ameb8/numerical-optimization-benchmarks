#ifndef POPULATION_H
#define POPULATION_H

#include "Problem/Problem.h"
#include <vector>

class Population { 
private:
    const int n; // Population size (number of solutions)
    const int m; // Dimension of each solution vector
    std::vector<std::vector<double>> solutions; // Size n x m matrix of solution vectors
public:
    // Constructors
    Population(int populationSize, int dimension);

    // Methods
    int initialize(double lower, double upper, int seed);
    std::vector<double> evaluate(const Problem& problem);

    // Accessors
    const std::vector<std::vector<double>>& getSolutions() const;
};

#endif