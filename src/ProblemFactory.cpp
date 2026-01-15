#include "ProblemFactory.h"
#include "Problem/Schwefel.h"   // Your concrete Problem implementation
#include "Problem/DeJongOne.h"    // Your concrete Problem implementation
//#include "Rosenbrock.h"
//#include "Rastrigin.h"
//#include "Griewangk.h"
//#include "SineEnvelope.h"
//#include "StretchedV.h"
//#include "AckleyOne.h"
//#include "AckleyTwo.h"
//#include "EggHolder.h"

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

// Array mapping integer IDs -> factory functions
constexpr std::array<Creator, 2> creators = {
    &createSchwefel,   // ID 0
    &createDeJongOne   // ID 1

};

} // anonymous namespace

std::unique_ptr<Problem> ProblemFactory::create(int id) {
    if (id < 0 || static_cast<std::size_t>(id) >= creators.size())
        throw std::out_of_range("Invalid problem ID");

    return creators[id](); // Call the factory function
}