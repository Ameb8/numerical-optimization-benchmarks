#include "RunExperiments.h"

#include <iostream>
#include <fstream>
#include <memory> 
#include <optional>

#include <External/json.hpp>

#include "ProblemFactory.h"
#include "Problem/Problem.h"
#include "SolutionBuilder.h"
#include "Optimizer/OptimizerFactory.h"
#include "debug.h"


using json = nlohmann::json; // Alias JSON parsing library



bool RunExperiments::loadConfig(const std::string& inputFile) {
    debug::log("\nConfig Loading from:\t", inputFile);

    std::ifstream file(inputFile);
    
    if(!file) { // File failed to open
        std::cerr << "Cannot open input file: " << inputFile << "\n";
        return false;
    }

    json j;
    file >> j;

    // Ensure experiments exist
    if(!j.contains("experiments") || !j["experiments"].is_array()) {
        std::cerr << "JSON does not contain 'experiments' array.\n";
        return false;
    }

    std::vector<ExperimentConfig> experiments; // Stores experiment configs

    for(const auto& item : j["experiments"]) {
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
            cfg.maxIterations = opt.value("iterations", 1);
            cfg.neighborDelta = opt.value("delta", 0.0);
            cfg.numNeighbors = opt.value("num_neighbors", 0);
        } else { // No optimizer provided
            cfg.optimizer = "";
            cfg.maxIterations = 0;
            cfg.neighborDelta = 0.0;
            cfg.numNeighbors = 0;
        }

        experiments.push_back(cfg);

        debug::log(
            "\n\nExperiment Config Created for ",
            cfg.experimentName, "\t(", cfg.problemType, ")",
            "\n Range: [", cfg.lower, ", ", cfg.upper, "]",
            "\nDimensions: ", cfg.dimensions,
            "\nSeed: ", cfg.seed,
            "\nOptimizer: ", cfg.optimizer, 
            "\nIterations: ", cfg.maxIterations,
            "\nNeighbors/Max Delta: ", cfg.numNeighbors,
            ", ", cfg.neighborDelta
        );
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

    for(size_t i = 0; i < configs.size(); i++) {
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
        debug::log("\nRunning Experiment:\t", configs[i].experimentName);
        ExperimentConfig& config = configs[i];

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
    std::vector<std::vector<double>> timeWriteCSV(runtimes.size());

    // Convert runtimes to csv-friendly format
    for(size_t i = 0; i < runtimes.size(); i++)
        timeWriteCSV[i] = { runtimes[i] };

    // Write fitness data to csv
    writeCSV(
        bestFitnessesPath,
        fitnessResults,
        experimentNames
    );

    // Write time data to csv
    writeCSV(
        timesPath,
        timeWriteCSV,
        experimentNames,
        std::vector<std::string>(1, "Execution Time")
    );

    return numExperiments;
}