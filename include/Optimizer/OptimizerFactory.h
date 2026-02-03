#ifndef OPTIMIZER_FACTORY_H
#define OPTIMIZER_FACTORY_H

#include "Optimizer/Optimizer.h"
#include "Optimizer/Blind.h"
#include "Optimizer/LocalSearch.h"

#include <memory>
#include <string>


class OptimizerFactory {
public:
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