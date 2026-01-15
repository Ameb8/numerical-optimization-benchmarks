#include <iostream>
#include <string.h>
#include "TestRunner.h"


int main(int argc, char* argv[]) {
    std::string configFile = "config.toml";
    
    if(argc > 1)
        configFile = argv[1];

    TestRunner testRunner(configFile);
    int numTests = testRunner.runTests();

    std::cout << "\n" << numTests << "Experiments executed successfully\n";
    
    return 0;
}