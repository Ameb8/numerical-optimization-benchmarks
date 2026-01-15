#include "Experiment.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include <chrono>
#include <iomanip>
#include <sstream>


#include "ProblemFactory.h"


#define ERROR 0
#define SUCCESS 1
#define OUT_FILE "results.csv" // Temporary filepath


int Experiment::writeJSON(const std::filesystem::path& dir,
                               const std::string& timestamp,
                               const Problem& problem,
                               const Config& config)
{
    std::ofstream file(dir / "experiment.json"); // Open file
    
    if(!file.is_open()) // Error opening file
        return ERROR;

    // Write Experiment metadata as JSON
    file << "{\n";
    file << "  \"ran_at\": \"" << timestamp << "\",\n";
    file << "  \"problem\": {\n";
    file << "    \"name\": \"" << problem.getName() << "\",\n";
    file << "    \"lower_bound\": " << problem.getLowerBound() << ",\n";
    file << "    \"upper_bound\": " << problem.getUpperBound() << ",\n";
    file << "    \"dimension\": " << config.dimension << "\n";
    file << "  },\n";
    file << "  \"config\": {\n";
    file << "    \"population_size\": " << config.populationSize << ",\n";
    file << "    \"seed\": " << config.seed << "\n";
    file << "  }\n";
    file << "}\n";

    return SUCCESS;
}


std::string Experiment::getTimestamp()
{
    // Initialize time
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    // Get time (unix-compatible only at the moment)
    std::tm tm{};
    localtime_r(&t, &tm);

    // Write current time
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    
    return oss.str(); // Return as string
}



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
    namespace fs = std::filesystem;

    std::string timestamp = getTimestamp();

    fs::path resultsDir = fs::current_path() / "results";
    fs::path timestampDir = resultsDir / timestamp;

    try {
        fs::create_directories(timestampDir);
    } catch (const fs::filesystem_error&) {
        return ERROR;
    }

    // Write CSV
    fs::path csvPath = timestampDir / OUT_FILE;
    if(!writeCSV(results, csvPath.string()))
        return ERROR;

    // Write JSON metadata
    if(!writeJSON(timestampDir, timestamp, problem, config))
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
    return writeResults(results, *problem, config);
}