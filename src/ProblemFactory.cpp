#include "ProblemFactory.h"
#include "Problem/Schwefel.h"
#include "Problem/DeJongOne.h"
#include "Problem/Rosenbrock.h"
#include "Problem/Rastrigin.h"
#include "Problem/Griewangk.h"
#include "Problem/SineEnvelope.h"
#include "Problem/StretchedV.h"
#include "Problem/AckleyOne.h"
#include "Problem/AckleyTwo.h"
#include "Problem/EggHolder.h"

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

std::unique_ptr<Problem> createGriewangk() {
    return std::make_unique<Griewangk>();
}

std::unique_ptr<Problem> createSineEnvelope() {
    return std::make_unique<SineEnvelope>();
}

std::unique_ptr<Problem> createStretchedV() {
    return std::make_unique<StretchedV>();
}

std::unique_ptr<Problem> createAckleyOne() {
    return std::make_unique<AckleyOne>();
}

std::unique_ptr<Problem> createAckleyTwo() {
    return std::make_unique<AckleyTwo>();
}

std::unique_ptr<Problem> createEggHolder() {
    return std::make_unique<EggHolder>();
}



// Array mapping integer IDs -> factory functions
constexpr std::array<Creator, 10> creators = {
    &createSchwefel,
    &createDeJongOne,
    &createRosenbrock,
    &createRastrigin,
    &createGriewangk,
    &createSineEnvelope,
    &createStretchedV,
    &createAckleyOne,
    &createAckleyTwo,
    &createEggHolder
};

} // anonymous namespace

std::unique_ptr<Problem> ProblemFactory::create(int id) {
    int index = id - 1; // Decrement index to array index

    // Get Creator function
    if (index < 0 || static_cast<std::size_t>(index) >= creators.size())
        throw std::out_of_range("Invalid problem ID");

    return creators[index](); // Call the factory function
}