#include "RunExperiments.h"

#include <iostream>
#include <fstream>
#include <memory> 
#include <optional>

#include <External/json.hpp>

#include <ProblemFactory.h>
#include <Problem/Problem.h>
#include <SolutionBuilder.h>

#include "Optimizer/OptimizerFactory.h"


using json = nlohmann::json; // Alias JSON parsing library



bool RunExperiments::loadConfig(const std::string& inputFile) {
    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "Cannot open input file: " << inputFile << "\n";
        return false;
    }

    json j;
    file >> j;

    if(!j.contains("experiments") || !j["experiments"].is_array()) {
        std::cerr << "JSON does not contain 'experiments' array.\n";
        return false;
    }

    std::vector<ExperimentConfig> experiments;

    for (const auto& item : j["experiments"]) {
        ExperimentConfig cfg;

        // Basic fields
        cfg.experimentName = item.value("experiment_name", "");
        cfg.problemType = item.value("problem_type", 0);
        cfg.dimensions = item.value("dimensions", 0);
        cfg.lower = item.value("lower_bound", 0.0);
        cfg.upper = item.value("upper_bound", 0.0);
        cfg.seed = item.value("seed", 1);

        // Optimizer fields
        if (item.contains("optimizer") && item["optimizer"].is_object()) {
            const auto& opt = item["optimizer"];
            cfg.optimizer = opt.value("type", "");
            cfg.maxIterations = opt.value("iterations", 0);
            cfg.neighborDelta = opt.value("delta", 0.0);
            cfg.numNeighbors = opt.value("num_neighbors", 0);
        } else { // No optimizer provided
            cfg.optimizer = "";
            cfg.maxIterations = 0;
            cfg.neighborDelta = 0.0;
            cfg.numNeighbors = 0;
        }

        experiments.push_back(cfg);
    }

    // Store loaded configs as field
    this->configs = experiments;

    return true;
}
/*
bool RunExperiments::runExperiment(ExperimentConfig& config) {
    // Perform experiment setup
    std::unique_ptr<Problem> problem = ProblemFactory::create(config.problemType);
    SolutionBuilder builder(config.dimensions, config.upper, config.lower, config.seed);
    std::unique_ptr<Optimizer> optimizer = OptimizerFactory::initOptimizer(*problem, config, builder);

    // Perform experiment
    double execTime = optimizer->optimize();
    
    writeResults(

        optimizer->getBestSolution(),
        optimizer->getBestFitnesses(),
        optimizer->getSolutions()
    );
}
*/

bool writeCSV(
    const std::string& filename,
    const std::vector<std::vector<double>>& data,
    const std::optional<std::vector<std::string>>& rowLabels = std::nullopt,
    const std::optional<std::vector<std::string>>& colLabels = std::nullopt
) {
    std::ofstream file(filename); // Open file to write
    
    if(!file.is_open()) // Error opening file
        return false;

    // Write column labels
    if(colLabels) {
        if(rowLabels) // top-left empty cell if row and col labels
            file << ","; 
        
        // Write all labels
        for(size_t j = 0; j < colLabels->size(); ++j) {
            if (j > 0) file << ",";
            file << (*colLabels)[j];
        }
        
        file << "\n";
    }

    // Write data rows
    for (size_t i = 0; i < data.size(); ++i) {
        if(rowLabels)
            file << (*rowLabels)[i] << ",";

        for(size_t j = 0; j < data[i].size(); ++j) {
            if (j > 0) file << ",";
            file << data[i][j];
        }

        file << "\n";
    }

    return true;
}


std::vector<std::string> RunExperiments::getNames(std::vector<ExperimentConfig> configs) {
    std::vector<std::string> names(configs.size());

    for(int i = 0; i < configs.size(); i++) {
        names[i] = configs[i].experimentName;
    }

    return names;
}



int RunExperiments::runExperiments() {
    int numExperiments = configs.size();

    // Create vectors to store all experiment results
    std::vector<double> runtimes(numExperiments);
    std::vector<std::vector<double>> fitnessResults(numExperiments);

    for(int i = 0; i < numExperiments; i++) {
        ExperimentConfig config = configs[i];

        // Perform experiment setup
        std::unique_ptr<Problem> problem = ProblemFactory::create(config.problemType);
        SolutionBuilder builder(config.dimensions, config.upper, config.lower, config.seed);
        std::unique_ptr<Optimizer> optimizer = OptimizerFactory::initOptimizer(*problem, config, builder);

        // Perform experiment
        runtimes[i] = optimizer->optimize();
        fitnessResults[i] = optimizer->getBestFitnesses();
    }

    std::vector<std::string> experimentNames = getNames(configs);
    std::string bestFitnessesPath = outputFile + "/" + std::string(bestFitnessesFile);
    std::string timesPath = outputFile + "/" + std::string(timesFile);

    // Write fitness data to csv
    writeCSV(
        bestFitnessesPath,
        fitnessResults,
        experimentNames
    );

    // Write time data to csv
    //writeCSV(

    //)

    return numExperiments;
}