/**
 * @file OptimizerFactory.h
 * @author Alex Buckley
 * @ingroup Optimizers
 * @brief Factory utility for instantiating different optimizer types.
 */


#ifndef OPTIMIZER_FACTORY_H
#define OPTIMIZER_FACTORY_H

#include "Optimizer/Optimizer.h"
#include "Optimizer/Blind.h"
#include "Optimizer/LocalSearch.h"

#include <memory>
#include <string>


/**
 * @class OptimizerFactory
 * @brief Factory class for creating optimizer instances.
 *
 * The OptimizerFactory encapsulates the logic for selecting and
 * constructing the appropriate optimization algorithm based on
 * experimental configuration parameters.
 */
class OptimizerFactory {
public:
    /**
     * @brief Initializes an optimizer based on configuration settings.
     *
     * @param problem Reference to the optimization problem.
     * @param config Experiment configuration specifying optimizer type and parameters.
     * @param builder Reference to the solution builder.
     *
     * @return A unique pointer to the initialized Optimizer instance,
     *         or nullptr if the optimizer type is unsupported.
     */
    static std::unique_ptr<Optimizer> initOptimizer(Problem& problem, ExperimentConfig& config, SolutionBuilder& builder) {
        if(config.optimizer == "blind")
            return std::make_unique<Blind>(builder, problem, config.maxIterations);
        else if(config.optimizer == "local")
            return std::make_unique<LocalSearch>(builder, problem, 1, config.neighborDelta, config.numNeighbors);
        else if(config.optimizer == "repeated local")
            return std::make_unique<LocalSearch>(builder, problem, config.maxIterations, config.neighborDelta, config.numNeighbors);
        
        return nullptr;
    }

};

#endif