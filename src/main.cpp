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