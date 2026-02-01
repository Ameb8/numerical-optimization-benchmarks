#include "Optimizer/LocalSearch.h"

#include <limits>

void LocalSearch::localSearch() {
    // Get initial population pseudo-randomly
    solutions.push_back(solutionBuilder.getRand());

    // Set initial solution as best
    bestFitnesses.push_back(problem.evaluate(solutions.back()));
    bestSolution = solutions.back();

    bool minimaFound = false;

    while(!minimaFound) {
        // Add current best solution/fitness as next iteration's best
        solutions.push_back(solutions.back());
        bestFitnesses.push_back(bestFitnesses.back());

        minimaFound = true;


        // Get set of neighbors
        std::vector<std::vector<double>> neighbors = solutionBuilder.getNeighbors(
            solutions.back(),
            numNeighbors,
            delta
        );



        // Check all neighbors
        for(size_t i = 0; i < neighbors.size(); i++) {
            double fitness = problem.evaluate(neighbors[i]);

            // Better fitness found, update stats
            if(fitness < bestFitnesses.back()) {
                bestFitnesses.back() = fitness;
                solutions.back() = neighbors[i];
            }

        }
        
    }


  
}



double LocalSearch::optimize() {
    // Start timing
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    for(int i = 0; i < maxIterations; i++)
        localSearch();

    return std::chrono::duration<double>(clock::now() - start).count();
}