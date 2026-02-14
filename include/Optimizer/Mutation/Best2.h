#ifndef BEST2_H
#define BEST2_H

#include "Optimizer/Mutation/Mutation.h"

class Best2 : public Mutation {
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
