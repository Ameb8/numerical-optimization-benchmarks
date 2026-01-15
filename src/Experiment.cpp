#include "Experiment.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include "ProblemFactory.h"


#define ERROR 0
#define SUCCESS 1
#define OUT_FILE "results.csv" // Temporary filepath


int Experiment::writeCSV(const std::vector<double>& data, const std::string& filepath) {
    std::ofstream outFile(filepath); // Open file

    if(!outFile.is_open()) // Error opening file
        return ERROR;

    // Write data
    for(size_t i = 0; i < data.size(); ++i) {
        outFile << data[i]; // Write data

        if (i != data.size() - 1) // Write comma
            outFile << ","; 
    }
    outFile << "\n"; 

    return SUCCESS;
}

int Experiment::writeResults(const std::vector<double>& results, 
                             const Problem& problem, 
                             const Config& config)
{
    if(!writeCSV(results, OUT_FILE))
        return ERROR;

    
    return SUCCESS;
}

int Experiment::runExperiment(const std::string& configFile) {
    // Read and initialize config file
    Config config;

    if(!config.loadFromFile("config.toml")) {
        std::cerr << "Failed to load configuration file\n";
        return ERROR;
    }

    // Create problem with config's Problem ID as unique pointer
    auto problem = ProblemFactory::create(config.problemType);
        
    // Create and initialize population
    Population population(config.populationSize, config.dimension);
    population.initialize(problem->getLowerBound(), problem->getUpperBound(), config.seed);

    // Evaluate problem
    std::vector<double> results = population.evaluate(*problem);
    
    // Write results
    writeResults(results, *problem, config);
}