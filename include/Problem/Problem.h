#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>

// Base class for all optimization benchmark problems
// Allows use of runtime polymorphism to evaluate different problems
class Problem {
protected:
    const double lowerBound;  // Lower bound of input values
    const double upperBound;  // Upper bound of input values
    const char* const name;   // Function name

public:
    // Constructor with member-initializer list
    Problem(double lb, double ub, const char* n)
        : lowerBound(lb), upperBound(ub), name(n) {}

    // Virtual destructor
    virtual ~Problem() = default;

    // Evaluate a single solution vector x
    // Returns the scalar fitness value
    virtual double evaluate(const std::vector<double>& x) const = 0;

    double getLowerBound() const { return lowerBound; }
    double getUpperBound() const { return upperBound; }
    const char* getName() const { return name; }
};

#endif // PROBLEM_H
