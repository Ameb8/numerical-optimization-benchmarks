#ifndef POPULATION_H
#define POPULATION_H

#include "Problem/Problem.h"

class Population {
private:
    int n; // Population size (number of solutions)
    int m; // Dimension of each solution vector
    std::vector<double> solutions; // Flattened n x m matrix of solution vectors
    std::vector<double> fitness; // Vector of fitness values, length n
public:
    // Constructors
    Population(int populationSize, int dimension);

    // Methods
    void initialize(double lower, double upper, int seed);
    void evaluate(const Problem& problem);

    // Accessors
    const std::vector<std::vector<double>>& getSolutions() const;
    const std::vector<double>& getFitness() const;
};ß

#endif