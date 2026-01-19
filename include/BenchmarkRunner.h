#ifndef BENCHMARK_RUNNER_H
#define BENCHMARK_RUNNER_H


#include "External/json.hpp"

#include <vector>
#include <string>

#include "Experiment.h"

using json = nlohmann::json;

class BenchmarkRunner {
private:
    inline static constexpr std::string fitnessCSV = "fitness.csv";
    inline static constexpr std::string timeCSV = "time.csv";
    
    json loadJSON(const std::string& path);
    Experiment parseExperiment(const json& j);
    std::vector<Experiment> parseExperiments(const json& cfg);
    void writeFitnessCSV(const std::vector<Experiment>& experiments, const std::string& filename);
    void writeTimeCSV(const std::vector<Experiment>& experiments,const std::string& filename);
public:
    void runBenchmarks(const std::string& inputFile, const std::string& benchmarkName);

};

#endif