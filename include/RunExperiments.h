#ifndef RUN_EXPERIMENTS_H
#define RUN_EXPERIMENTS_H

#include <vector>
#include <string>

#include "Config.h"
#include "Optimizer/Optimizer.h"


class RunExperiments {
private:
    std::vector<ExperimentConfig> configs;
    std::string outputFile;

    bool loadConfig(const std::string& inputFile);
    //bool runExperiment(ExperimentConfig& experiment);
    std::vector<std::string> getNames(std::vector<ExperimentConfig> configs);
    bool writeResults(
        std::vector<double> bestSolution, 
        std::vector<double> bestFitnesses, 
        std::vector<std::vector<double>> solutions
    );
    
    // Output file names
    static constexpr std::string_view bestFitnessesFile = "best_fitnesses.csv";
    static constexpr std::string_view solutionsFile = "solutions.csv";
    static constexpr std::string_view timesFile = "times.csv";

public:
    RunExperiments(const std::string inputFile, const std::string& outputFile)
        : outputFile(outputFile)
    {
        loadConfig(inputFile);
    }

    int runExperiments();

};



#endif
