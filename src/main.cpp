/**
 * @file main.cpp
 * @author Alex Buckley
 * @ingroup Core
 * @brief Entry point for the Numerical Optimization Benchmarks CLI.
 * * This file handles the command-line interface, initializes the debugging 
 * environment, and hands off execution to the RunExperiments controller.
 * * @section usage_sec Usage
 * @code
 * ./optimization_benchmarks <config_path> <output_path>
 * @endcode
 * * @param argc Argument count.
 * @param argv Argument vector. Expects [1] config path and [2] output path.
 * @return int Status code (0 for success, 1 for error).
 */


#include <iostream>
#include <string.h>

#include "RunExperiments.h"
#include "debug.h"

int main(int argc, char* argv[]) {
    // Debug print
    debug::log("\nDebug Mode Enabled For:\t", argv[0]);

    if(argc < 3) { // Read program arguments
        std::cerr <<"Error, not enough arguments provided\n";
        return 1;
    }

    RunExperiments runner(argv[1], argv[2]);
    runner.runExperiments();

    return 0;
}