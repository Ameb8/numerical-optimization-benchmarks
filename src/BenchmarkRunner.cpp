#include "BenchmarkRunner.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>


using json = nlohmann::json;


json BenchmarkRunner::loadJSON(const std::string& path) {
    std::ifstream f(path);
    
    if(!f)
        throw std::runtime_error("Failed to open config file: " + path);
    else
        std::cout << "Opened file: " << path << "\n";

    return json::parse(f);
}


Experiment BenchmarkRunner::parseExperiment(const json& j) {
    // Parse data from JSON
    std::string name = j.at("name").get<std::string>();
    int problemType = j.at("problem_type").get<int>();
    int popSize = j.at("population_size").get<int>();
    int dims = j.at("dimensions").get<int>();
    int upper = j.at("upper_bound").get<int>();
    int lower = j.at("lower_bound").get<int>();
    unsigned int seed = j.at("seed").get<unsigned int>();
    
    // Return Experiment object
    return Experiment(
        name,
        problemType,
        popSize,
        dims,
        seed,
        lower,
        upper
    );
}


std::vector<Experiment> BenchmarkRunner::parseExperiments(const json& cfg) {
    std::vector<Experiment> experiments;

    const auto& arr = cfg.at("experiments");
    
    if(!arr.is_array())
        throw std::runtime_error("'experiments' must be an array");

    for(const auto& jexp : arr)
        experiments.push_back(parseExperiment(jexp));

    return experiments;
}


void BenchmarkRunner::writeFitnessCSV(const std::vector<Experiment>& experiments, const std::string& filename) {
    if (experiments.empty()) {
        throw std::runtime_error("No experiments to write");
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file");
    }

    // Assume all experiments have same fitness vector size
    const size_t fitnessCount = experiments.front().getFitness().size();

    // Write experiment names as column headers
    for (size_t j = 0; j < experiments.size(); ++j) {
        file << experiments[j].getName(); // Write name
        if(j + 1 < experiments.size())
            file << ","; // Write delimiter
    }
    
    file << "\n"; // Next line

    // Write rows (fitness values)
    for (size_t i = 0; i < fitnessCount; ++i) { // Iterate through fitness value
        // Write fitness__i for each experiment
        for (size_t j = 0; j < experiments.size(); ++j) {
            const auto& fitness = experiments[j].getFitness(); // Get fitness values

            if(fitness.size() != fitnessCount) // Size error
                throw std::runtime_error("Fitness size mismatch between experiments");

            file << fitness[i]; // Write fitness__i for experiment__j

            if(j + 1 < experiments.size())
                file << ","; // Write delimiter
        }
        
        file << "\n"; // Write newline
    }

    file.close();
}


void BenchmarkRunner::writeTimeCSV(const std::vector<Experiment>& experiments, const std::string& filename) {
    if(experiments.empty()) // Missing experiments
        throw std::runtime_error("No experiments to write");

    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file");

    // Write header to CSV
    file << "experiment_name, wall_time\n";

    // Write experiment name and wall time to CSV
    for(size_t i = 0; i < experiments.size(); i++)
        file << experiments[i].getName() << ", " << experiments[i].getWallTime() << "\n";

}

void BenchmarkRunner::runBenchmarks(const std::string& inputFile, const std::string& benchmarkName) {
    constexpr std::string fitnessName = "fitness.csv";
    constexpr std::string timeName = "time.csv";

    json j = loadJSON(inputFile);
    std::vector<Experiment> experiments = parseExperiments(j);

    // Run each experiment
    for(size_t i = 0; i < experiments.size(); i++)
        experiments[i].runExperiment();

    namespace fs = std::filesystem; // Alias filesystem namespace
    fs::path resultDir = fs::current_path() / "results" / benchmarkName; // Get path to results directory

    writeFitnessCSV(experiments, resultDir / fitnessCSV);
    writeTimeCSV(experiments, resultDir / timeCSV);


}
