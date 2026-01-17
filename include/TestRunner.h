#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <vector>
#include <string>
#include "Config.h"

class TestRunner {
private:
    std::vector<Config> experiments;
    std::string testName;

    int loadFromFile(const std::string& configFile);
public:
    TestRunner(const std::string& configFile);
    int runTests();

}; 

#endif