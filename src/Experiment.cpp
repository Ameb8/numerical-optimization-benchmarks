#include "Experiment.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


#include <chrono>
#include <iomanip>
#include <sstream>


#include "ProblemFactory.h"




void Experiment::runExperiment() {
    // Initialize clock
    using clock = std::chrono::steady_clock;

    // Time evaluation 
    const auto start = clock::now();
    fitness = population.evaluate(*problem);
    const auto end = clock::now();

    // Set wall time field
    wallTime = std::chrono::duration<double>(end - start).count();
}



