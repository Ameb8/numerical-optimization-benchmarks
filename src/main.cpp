#include <iostream>
#include <string.h>
#include "RunExperiments.h"

int main(int argc, char* argv[]) {
    if(argc < 3) { // Read program arguments
        std::cerr <<"Error, not enough arguments provided\n";
        return 1;
    }

    RunExperiments runner(argv[1], argv[2]);
    runner.runExperiments();

    return 0;
}