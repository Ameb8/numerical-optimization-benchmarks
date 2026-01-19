#include <iostream>
#include <string.h>
#include "BenchmarkRunner.h"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cerr <<"Error, not enough arguments provided\n";
        return 1;
    }

    BenchmarkRunner br;
    br.runBenchmarks(argv[1], argv[2]);

    /*
    std::string configFile = "config.toml";
    
    if(argc > 1)
        configFile = argv[1];

    TestRunner testRunner(configFile);
    int numTests = testRunner.runTests();

    std::cout << "\n" << numTests << " Experiments executed successfully\n";
    */
    return 0;
}