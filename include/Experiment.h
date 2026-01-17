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
    bool writeCSV(const std::vector<double>& data, const std::string& filepath);
    bool writeJSON(const std::filesystem::path& dir,
                   const std::string& timestamp,
                   const std::string& problemName,
                   const Config& config);
    bool writeResults(const std::vector<double>& results, 
                      const Problem& problem, 
                      const Config& config,
                      const std::string runName);
public:
    bool runExperiment(Config config, std::string runName);
};


#endif 