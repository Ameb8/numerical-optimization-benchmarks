#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>

// Base class for all optimization benchmark problems
// Allows use of runtime polymorphism to evaluate different problems
class Problem {
public:
    virtual ~Problem() = default;  // Always provide virtual destructor

    // Evaluate a single solution vector x
    // Returns the scalar fitness value
    virtual double evaluate(const std::vector<double>& x) const = 0;

    // Get lower bound for each variable
    virtual double getLowerBound() const = 0;

    // Get upper bound for each variable
    virtual double getUpperBound() const = 0;

    // Get name of the problem
    virtual const char* getName() const = 0;
};

#endif // PROBLEM_H
