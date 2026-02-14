#include "Optimizer/DifferentialEvolution.h"

#include <limits>

std::vector<std::vector<double>> DifferentialEvolution::initPopulation() {
    std::vector<std::vector<double>> pop(popSize);

    for(int i = 0; i < popSize; i++) {
        pop[i] = solutionBuilder.getRand();
    }

    return pop;
}



double DifferentialEvolution::optimize() {
    // Allocate memory to store best fitness per iteration
    bestFitnesses.resize(maxIterations, std::numeric_limits<double>::max());
    
    // Start timing
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    // Randomly initialize population
    std::vector<std::vector<double>> pop = initPopulation();
    
    // Stores initial fitness values
    std::vector<double> initFitness(popSize);
    
    for(int i = 0; i < maxIterations; i++) {
        // Temporarily stores new population
        std::vector<std::vector<double>> newPop = pop;

        // Calculate fitnesses for initial population
        for(int j = 0; j < popSize; j++) {
            // Get mutated vector
            std::vector<double> mutated =
                mutStrat->mutate(pop, j, scale, bestVector, solutionBuilder);
                
            // Create crossover vector
            crossStrat->crossover(newPop[j], mutated, crossover, solutionBuilder);

            // Calculate fitness of original and trial vectors
            double oldFitness = problem.evaluate(pop[j]);
            double newFitness = problem.evaluate(newPop[j]);

            // Trial vector less optimal
            if(oldFitness <= newFitness) {
                newPop[j] = pop[j];

                // Update iteration fitness
                if(oldFitness < bestFitnesses[i])
                    bestFitnesses[i] = oldFitness;
            } else if(newFitness < bestFitnesses[i]) {
                bestFitnesses[i] = newFitness;
            }

        }

        pop = newPop;

    }


    return std::chrono::duration<double>(clock::now() - start).count();
}

