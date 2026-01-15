#include "Config.h"

#include <iostream>


/*
    // Experiment name
    std::string experiment;

    // Population
    int populationSize;
    int lowerBound;
    int upperBound;

    // Problem
    int problemType;
    int dimension;

    // Random
    unsigned int seed;
*/

bool Config::loadFromTable(const toml::table& tbl) {
    experimentName = tbl["experiment_name"].value_or("");
    populationSize = tbl["population_size"].value_or(30);
    lowerBound = tbl["lowerBound"].value_or(1.0);
    upperBound = tbl["upperBound"].value_or(-1.0);
    problemType = tbl["problem_type"].value_or(1);
    dimension = tbl["dimension"].value_or(30);
    seed = tbl["seed"].value_or(5489u);
    
    return true;
}

