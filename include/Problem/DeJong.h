#ifndef DEJONG_H
#define DEJONG_H

#include "Problem.h"
#include <vector>

#define PROBLEM_NAME "DeJong"
#define LOWER_BOUND -100.0
#define UPPER_BOUND 100.0

class DeJong : public Problem {
private:
    static constexpr double LOWER = -100.0;
    static constexpr double UPPER = 100.0;
    static constexpr const char* NAME = "DeJong";

public:
    // Constructor method
    DeJong() : Problem(LOWER, UPPER, NAME) {}
    
    // Evaluate function
    double evaluate(const std::vector<double>& x) const override {
        double sum = 0.0;

        for (double xi : x)
            sum += xi * xi;
        
        return sum;
    }
};

#endif
