#include <iostream>
#include <string.h>
#include "BenchmarkRunner.h"

int main(int argc, char* argv[]) {
    if(argc < 3) { // Read program arguments
        std::cerr <<"Error, not enough arguments provided\n";
        return 1;
    }

    // Run benchmark
    BenchmarkRunner br;
    br.runBenchmarks(argv[1], argv[2]);

    return 0;
}