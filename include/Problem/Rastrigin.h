#ifndef RASTRIGIN_H
#define RASTRIGIN_H

#include "Problem/Problem.h"
#include <vector>
#include <cmath>

class Rastrigin : public Problem {
private:
    static constexpr double LOWER = -30.0;
    static constexpr double UPPER = 30.0;
    static constexpr std::string_view NAME = "Rastrigin";

public:
    Rastrigin() : Problem(LOWER, UPPER, NAME) {}

    double evaluate(const std::vector<double>& x) const override {
        double sum = 0.0;
        
        for(double xi : x) // Calculate summation
            sum += xi * xi - 10 * std::cos(2 * M_PI * xi);

        return 10 * x.size() + sum;
    }
};

#endif
