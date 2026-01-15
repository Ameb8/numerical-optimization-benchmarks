#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>
#include <filesystem>

#include "Config.h"
#include "Population.h"
#include "Problem/Problem.h"

class Experiment { 
private:
    std::string getTimestamp();
    int writeCSV(const std::vector<double>& data, const std::string& filepath);
    int writeJSON(const std::filesystem::path& dir,
                               const std::string& timestamp,
                               const Problem& problem,
                               const Config& config);
    int writeResults(const std::vector<double>& results, 
                const Problem& problem, 
                const Config& config);
public:
    int runExperiment(const std::string& configFile);
};


#endif 