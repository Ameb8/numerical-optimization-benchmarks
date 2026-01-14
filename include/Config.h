#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    // Population
    int populationSize;

    // Problem
    int problemType;
    int dimension;

    // Random
    unsigned int seed;

    // Load from TOML
    bool loadFromFile(const std::string& filename);
};

#endif