#include "Config.h"
#include "toml.hpp"
#include <iostream>

#define DEFAULT_POPULATION_SIZE 30
#define DEFAULT_PROBLEM_TYPE 1
#define DEFAULT_DIMENSIONS 30
#define DEFAULT_RAND_SEED 5489u

bool Config::loadFromFile(const std::string& filename)
{
    try {
        toml::table tbl = toml::parse_file(filename);

        // Population
        populationSize =
            tbl["population"]["size"].value_or(DEFAULT_POPULATION_SIZE);

        // Problem
        problemType =
            tbl["problem"]["type"].value_or(DEFAULT_PROBLEM_TYPE);

        dimension =
            tbl["problem"]["dimension"].value_or(DEFAULT_DIMENSIONS);

        // Random
        seed =
            tbl["random"]["seed"].value_or(DEFAULT_RAND_SEED);


    }
    catch (const toml::parse_error& err) {
        std::cerr << "TOML parse error:\n"
                  << err << std::endl;
        return false;
    }

    return true;
}
