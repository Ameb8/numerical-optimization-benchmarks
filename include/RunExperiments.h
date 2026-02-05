/**
 * @file RunExperiments.h
 * @defgroup Core Core Engine
 * @brief The heart of the application that manages configuration and execution.
 * @author Alex Buckley
 * @ingroup Core
 */


#ifndef RUN_EXPERIMENTS_H
#define RUN_EXPERIMENTS_H

#include <vector>
#include <string>

#include "Config.h"
#include "Optimizer/Optimizer.h"


/**
 * @class RunExperiments
 * @brief High-level controller that orchestrates the benchmarking process.
 * * This class is responsible for:
 * 1. Parsing the JSON configuration file into ExperimentConfig structures.
 * 2. Iterating through experiments and instantiating the required Problems and Optimizers.
 * 3. Collecting performance metrics (fitness and runtime).
 * 4. Exporting results to CSV files for analysis.
 */
class RunExperiments {
private:
std::vector<ExperimentConfig> configs; ///< List of loaded experiment parameters
    std::string outputFile; ///< Destination directory for result files

    /**
     * @brief Parses a JSON file to populate the internal configs vector.
     * @param inputFile Path to the .json configuration file.
     * @return true if loading and parsing succeeded, false otherwise.
     */
    bool loadConfig(const std::string& inputFile);

    /**
     * @brief Extracts experiment names from a list of configurations.
     * @param configs The list of configurations to process.
     * @return A vector of strings containing only the names.
     */
    std::vector<std::string> getNames(std::vector<ExperimentConfig> configs);
    
    /**
     * @brief Internal helper to write experiment data to disk.
     * @param bestSolution Final best solution found.
     * @param bestFitnesses Convergence data (best fitness per iteration).
     * @param solutions All solutions evaluated.
     * @return true if the write was successful.
     */
    bool writeResults(
        std::vector<double> bestSolution, 
        std::vector<double> bestFitnesses, 
        std::vector<std::vector<double>> solutions
    );
    
    /** @name Output File Naming Defaults */
    ///@{
    static constexpr std::string_view bestFitnessesFile = "best_fitnesses.csv"; ///< CSV for convergence data
    static constexpr std::string_view solutionsFile = "solutions.csv";         ///< CSV for full solution set
    static constexpr std::string_view timesFile = "times.csv";                 ///< CSV for execution runtimes
    ///@}

public:
    /**
     * @brief Constructs the runner and immediately triggers configuration loading.
     * @param inputFile Path to the JSON configuration.
     * @param outputFile Path to the directory where results will be saved.
     */
    RunExperiments(const std::string inputFile, const std::string& outputFile)
        : outputFile(outputFile)
    {
        loadConfig(inputFile);
    }

    /**
     * @brief The main execution loop for all loaded experiments.
     * * Iterates through all configurations, initializes the Problem and Optimizer 
     * factories, runs the optimization, and triggers the CSV export.
     * * @return int The total number of experiments successfully processed.
     */
    int runExperiments();

};



#endif
