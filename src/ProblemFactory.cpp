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
constexpr std::array<Creator, 4> creators = {
    &createSchwefel,    // ID 1
    &createDeJongOne,   // ID 2
    &createRosenbrock,  // ID 3
    &createRastrigin    // ID 4

};

} // anonymous namespace

std::unique_ptr<Problem> ProblemFactory::create(int id) {
    int index = id - 1; // Decrement index to array index

    // Get Creator function
    if (index < 0 || static_cast<std::size_t>(index) >= creators.size())
        throw std::out_of_range("Invalid problem ID");

    return creators[index](); // Call the factory function
}