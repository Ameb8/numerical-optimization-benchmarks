#ifndef RANDBEST1_H
#define RANDBEST1_H

#include "Optimizer/Mutation/Mutation.h"

class RandBest1 : public Mutation {
public:
    std::vector<double> mutate(
        const std::vector<std::vector<double>>& population,
        int targetIndex,
        double F,
        const std::vector<double>& bestVector,
        SolutionBuilder& builder
    ) override;
};

#endif 
