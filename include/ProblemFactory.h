#ifndef PROBLEM_FACTORY_H
#define PROBLEM_FACTORY_H

#include "Problem/Problem.h"
#include <array>
#include <memory>
#include <stdexcept>


class ProblemFactory {
public:
    static std::unique_ptr<Problem> create(int id);
};


#endif