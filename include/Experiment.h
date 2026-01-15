#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>

#include "Config.h"
#include "Population.h"
#include "Problem.h"

class Experiment { 
private:
    int writeCSV(const std::vector<double>& data, const std::string& filepath);
    int writeResults(const std::vector<double>& results, 
                     const Problem& problem, 
                     const Config& config);
public:
    int runExperiment(const std::string& configFile);
};


#endif 