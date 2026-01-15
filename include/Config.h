#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "toml.hpp"

class Config {
public:
    // Experiment name
    std::string experimentName;

    // Population
    int populationSize;
    int lowerBound;
    int upperBound;

    // Problem
    int problemType;
    int dimension;

    // Random
    unsigned int seed;


    bool loadFromTable(const toml::table& tbl);
};

#endif