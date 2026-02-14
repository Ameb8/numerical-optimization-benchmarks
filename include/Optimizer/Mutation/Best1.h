#ifndef BEST1_H
#define BEST1_H

#include "Optimizer/Mutation/Mutation.h"

class Best1 : public Mutation {
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
