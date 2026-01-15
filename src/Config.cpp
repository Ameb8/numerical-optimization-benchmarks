#include "Config.h"

#include <iostream>

#define DEFAULT_POPULATION_SIZE 30
#define DEFAULT_PROBLEM_TYPE 1
#define DEFAULT_DIMENSIONS 30
#define DEFAULT_RAND_SEED 5489u


bool Config::loadFromTable(const toml::table& tbl) {
    populationSize = tbl["population_size"].value_or(30);
    problemType = tbl["problem_type"].value_or(1);
    dimension = tbl["dimension"].value_or(30);
    seed = tbl["seed"].value_or(5489u);
    
    return true;
}

