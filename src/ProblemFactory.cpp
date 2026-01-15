#include "ProblemFactory.h"
#include "Problem/Schwefel.h"
#include "Problem/DeJongOne.h"
#include "Problem/Rosenbrock.h"
#include "Problem/Rastrigin.h"
//#include "Problem/Griewangk.h"
//#include "Problem/SineEnvelope.h"
//#include "Problem/StretchedV.h"
//#include "Problem/AckleyOne.h"
//#include "Problem/AckleyTwo.h"
//#include "Problem/EggHolder.h"

namespace {

// Type alias for a factory function returning unique_ptr<Problem>
using Creator = std::unique_ptr<Problem>(*)();

// Factory functions for each problem
std::unique_ptr<Problem> createSchwefel() {
    return std::make_unique<Schwefel>();
}

std::unique_ptr<Problem> createDeJongOne() {
    return std::make_unique<DeJongOne>();
}

std::unique_ptr<Problem> createRosenbrock() {
    return std::make_unique<Rosenbrock>();
}

std::unique_ptr<Problem> createRastrigin() {
    return std::make_unique<Rastrigin>();
}

// Array mapping integer IDs -> factory functions
constexpr std::array<Creator, 2> creators = {
    &createSchwefel,    // ID 0
    &createDeJongOne,   // ID 1
    &createRosenbrock,  // ID 2
    &createRastrigin  // ID 3

};

} // anonymous namespace

std::unique_ptr<Problem> ProblemFactory::create(int id) {
    if (id < 0 || static_cast<std::size_t>(id) >= creators.size())
        throw std::out_of_range("Invalid problem ID");

    return creators[id](); // Call the factory function
}