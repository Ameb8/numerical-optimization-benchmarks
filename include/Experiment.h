#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>
#include <filesystem>

#include "Population.h"
#include "Problem/Problem.h"
#include "ProblemFactory.h"

class Experiment { 
private:
    const std::string name;
    const int problemType;
    const int populationSize;
    const int dimensions;

    std::unique_ptr<Problem> problem;
    Population population;
    std::vector<double> fitness;
    double wallTime;
public:
    Experiment(std::string name,
               int problemType,
               int popSize,
               int dims,
               unsigned int seed,
               int lower,
               int upper)
        : name(std::move(name)),
          problemType(problemType),
          populationSize(popSize),
          dimensions(dims),
          problem(ProblemFactory::create(problemType)),
          population(popSize, dims),
          wallTime(0.0)
    {
        population.initialize(lower, upper, seed);
    }

    void runExperiment();

    inline const std::string& getName() const { return name; }
    inline const std::vector<double>& getFitness() const { return fitness; }
    inline double getWallTime() const {return wallTime; }
};

#endif 