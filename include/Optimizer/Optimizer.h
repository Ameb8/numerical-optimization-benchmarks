/**
 * @file Optimizer.h 
 * @author Alex Buckley
 * @defgroup Optimizers Optimization Algorithms
 * @ingroup Optimizers
 * @brief Base class interface for all numerical optimization algorithms.
 */


#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <vector>

#include "Problem/Problem.h"
#include "SolutionBuilder.h"


/**
 * @class Optimizer
 * @brief Abstract base class for all optimization algorithms.
 *
 * The Optimizer class defines a common interface and shared state
 * for optimization algorithms operating on benchmark problems.
 * Derived classes must implement the optimize() method.
 * Results for each run are stored in fields and accessed through getter methods
 */
class Optimizer {
public:
    /**
     * @brief Constructs an Optimizer.
     *
     * @param solutionBuilder Reference to the solution generator.
     * @param problem Reference to the optimization problem.
     * @param maxIterations Maximum number of iterations allowed.
     */
    Optimizer(SolutionBuilder& solutionBuilder, Problem& problem, int maxIterations)
        : problem(problem),
          solutionBuilder(solutionBuilder),
          maxIterations(maxIterations)
    {}

    /// Virtual destructor for safe polymorphic deletion
    virtual ~Optimizer() = default;

    /**
     * @brief Runs the optimization algorithm.
     *
     * @return Execution time of the algorithm
     */
    virtual double optimize() = 0; 

    // Getters
    /** @name Accessors */
    ///@{

    /// @return The final best fitness value
    double getBestFitness() { return bestFitnesses.back(); }

    /// @return Reference to the best solution vector
    std::vector<double>& getBestSolution() { return bestSolution; }

    /// @return Best fitness value so far per iteration
    std::vector<double>& getBestFitnesses() { return bestFitnesses; }

    /// @return All solutions evaluated during optimization
    std::vector<std::vector<double>>& getSolutions() { return solutions; }

    /// @return Maximum number of iterations
    int getMaxIterations() { return maxIterations; }

    /// @return Reference to the optimization problem
    Problem& getProblem() { return problem; }

    /// @return Reference to the solution builder
    SolutionBuilder& getSolutionBuilder() { return solutionBuilder; }

    ///@}


protected:
    /// Optimization problem definition
    Problem& problem;
    
    /// Solution generation utility
    SolutionBuilder& solutionBuilder;

    /// Maximum number of iterations
    int maxIterations;

    /// Best solution found
    std::vector<double> bestSolution;

    /// Best fitness value so far per iteration
    std::vector<double> bestFitnesses;

    /// All generated solutions
    std::vector<std::vector<double>> solutions;
};

#endif