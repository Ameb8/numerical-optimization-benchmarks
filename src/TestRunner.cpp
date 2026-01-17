#include "TestRunner.h"

#include <iostream>

#include "toml.hpp"
#include "Experiment.h"



TestRunner::TestRunner(const std::string& configFile)
    : experiments{} // Initialize empty experiments vector
{   
    loadFromFile(configFile); // Load experiments from config file
}



int TestRunner::loadFromFile(const std::string& configFile) {
    try {
        // Parse file into toml::table object
        toml::table tbl = toml::parse_file(configFile);

        // Read test name from config
        if(auto testTbl = tbl["test"].as_table()) {
            if(auto name = testTbl->get_as<std::string>("test_name"))
                testName = name->get();
        }

        if(tbl.contains("experiment")) { // Ensure file contains experiments
            // Get experiment as array
            auto exps = tbl["experiment"].as_array();
            
            if(!exps) { // Invalid experiment structure
                std::cerr << "Error: 'experiment' must be an array of tables.\n";
                return false;
            }

            experiments.clear(); // Clear any loaded experiments

            for (auto& e : *exps) { // Iterate through experiments
                Config cfg; // Declare config object

                // Ensure entry is table
                if(auto tbl_ptr = e.as_table()) {
                    cfg.loadFromTable(*tbl_ptr); // Initialize Config object
                    experiments.push_back(cfg); // Add Config to experiments
                } else { // Experiments is not table
                    std::cerr << "Warning: experiment entry is not a table!\n";
                }
            }

        } else { // File does not contain experiments
            std::cerr << "No 'experiment' tables found in TOML.\n";
            return false;
        }
    }
    catch (const toml::parse_error& err) { // Error parsing toml
        std::cerr << "TOML parse error:\n" << err << std::endl;
        return false;
    }

    return experiments.size();
}

int TestRunner::runTests() {
    Experiment exp; // Experiment object
    int experimentsRan = 0; // Number of experiments ran

    // Run all experiments
    for(Config cfg : experiments) {
        if(exp.runExperiment(cfg, testName))
           experimentsRan++; 
    }

    // Return number of experiments ran
    return experimentsRan;
}
