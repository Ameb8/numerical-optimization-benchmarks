#ifndef DEJONG_H
#define DEJONG_H

#include "Problem/Problem.h"
#include <vector>



class DeJongOne : public Problem {
private:
    static constexpr double LOWER = -100.0;
    static constexpr double UPPER = 100.0;
    static constexpr std::string_view NAME = "DeJong_1";

public:
    // Constructor method
    DeJongOne() : Problem(LOWER, UPPER, NAME) {}
    
    // Evaluate function
    double evaluate(const std::vector<double>& x) const override {
        double sum = 0.0;

        for(double xi : x)
            sum += xi * xi;
        
        return sum;
    }
};

#endif
