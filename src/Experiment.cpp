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


bool Experiment::writeJSON(const std::filesystem::path& dir,
                               const std::string& timestamp,
                               const std::string& problemName,
                               const Config& config)
{
    std::ofstream file(dir / "experiment.json"); // Open file
    
    if(!file.is_open()) // Error opening file
        return ERROR;

    // Write Experiment metadata as JSON
    file << "{\n";
    file << "  \"experiment\": \"" << config.experimentName << "\",\n";
    file << "  \"ran_at\": \"" << timestamp << "\",\n";
    file << "  \"problem\": {\n";
    file << "    \"name\": \"" << problemName << "\",\n";
    file << "    \"lower_bound\": " << config.lowerBound << ",\n";
    file << "    \"upper_bound\": " << config.upperBound << ",\n";
    file << "    \"dimension\": " << config.dimension << "\n";
    file << "  },\n";
    file << "  \"config\": {\n";
    file << "    \"population_size\": " << config.populationSize << ",\n";
    file << "    \"seed\": " << config.seed << "\n";
    file << "  }\n";
    file << "}\n";

    return true;
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



bool Experiment::writeCSV(const std::vector<double>& data, const std::string& filepath) {
    std::ofstream outFile(filepath); // Open file

    if(!outFile.is_open()) // Error opening file
        return false;

    // Write data
    for(size_t i = 0; i < data.size(); ++i) {
        outFile << data[i]; // Write data

        if (i != data.size() - 1) // Write comma
            outFile << ","; 
    }
    outFile << "\n"; 

    return true;
}


bool Experiment::writeResults(const std::vector<double>& results, 
                             const Problem& problem, 
                             const Config& config)
{
    namespace fs = std::filesystem; // Alias filesystem namespace

    std::string timestamp = getTimestamp(); // Get timestamp

    // Get filepath to results directory
    fs::path resultsDir = fs::current_path() / "results";
    
    // Name dir with experiment name
    fs::path experimentDir = resultsDir / config.experimentName;
    
    // Use timestamp if no name provided
    if(config.experimentName.empty())
        fs::path experimentDir = resultsDir / timestamp;      
    
    int suffix = 2; // Create suffix version if timestamped dir exists

    while(fs::exists(experimentDir)) { // Append suffix until dir name is unique
        experimentDir = resultsDir / (timestamp + "__" + std::to_string(suffix));
        ++suffix;
    }

    try { // Create results directory
        fs::create_directories(experimentDir);
    } catch(const fs::filesystem_error&) {
        return false; // Error creating file
    }

    // Write CSV
    fs::path csvPath = experimentDir / OUT_FILE;
    
    // Error writing to csv
    if(!writeCSV(results, csvPath.string()))
        return false;

    // Write JSON metadata
    if(!writeJSON(experimentDir, timestamp, problem.getName(), config))
        return false;

    return true;
}

bool Experiment::runExperiment(Config config) {
    // Create problem with config's Problem ID as unique pointer
    auto problem = ProblemFactory::create(config.problemType);

    // Update config bounds if not provided
    if(config.upperBound < config.lowerBound) {
        config.lowerBound = problem->getLowerBound();
        config.upperBound = problem->getUpperBound();
    }
    
    // Create and initialize population
    Population population(config.populationSize, config.dimension);
    population.initialize(config.lowerBound, config.upperBound, config.seed);

    // Evaluate problem
    std::vector<double> results = population.evaluate(*problem);
    
    // Write results
    return writeResults(results, *problem, config);
}