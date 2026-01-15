# Numerical Optimization Benchmarks

## Overview

This repository contains a benchmarking framework for a selection of standard benchmark functions. The program pseudo-randomly initializes a population and evaluates fitness for a selected problem. Inputs and problem are configurable by the user through a [config.toml](https://github.com/Ameb8/numerical-optimization-benchmarks/blob/master/config.toml) file. Additionally, this repository includes a script for calculating statistics and generating visualizations based off benchmarking results. 

## Benchmarking Program

### Requirements

This program was designed to run on MacOS and Linux systems. As such, attempting to run on a Windows device may cause errors, even if the WSL Linux-subsystem is used. Additionally, the Makefile provided utilizes g++ to compile the program, thus, g++ must be installed on the user's system.

### Compiling the Program

The included [Makefile](https://github.com/Ameb8/numerical-optimization-benchmarks/blob/master/Makefile) allows for simple compilation of this program. To compile, the user must navigate to the root directory of this repository. It is then recommended (but not always required) that the user runs `make clean`. This should result in output something like `rm -rf build bin`. Then, the user simply runs the `make` command. If successful, the output should look something like:

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/Experiment.cpp -o build/Experiment.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/TestRunner.cpp -o build/TestRunner.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/Utils/mt.cpp -o build/Utils/mt.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/Population/Population.cpp -o build/Population/Population.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/Config.cpp -o build/Config.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/ProblemFactory.cpp -o build/ProblemFactory.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -c src/main.cpp -o build/main.o
g++ -std=c++17 -Wall -Wextra -Iinclude -O3 -o bin/benchmark build/Experiment.o build/TestRunner.o build/Utils/mt.o build/Population/Population.o build/Config.o build/ProblemFactory.o build/main.o
```

Additionally, the user can inspect the `bin/` directory to see if it contains an executable program named `benchmark` in order to validate build success. If this file appears, the program is ready for execution.

### Running the Program

Once the program has been compiled, you are ready to begin running benchmarks. Assuming the included Makefile was used, the executable should live in `bin/benchmark` relative to the repository's root directory. To run the program, you simply execute `./bin/benchmark` from the repository root. The program will output how many experiments were successfully run, and the results can be found in `results <experiment_name>/`.

The program can optionally take one command-line argument for providing a path to an alternate configuration file. If no argument is provided, the `config.toml` found in the repository root will be used. Otherwise, the user can provide a path to any appropriately-structure toml file. The `.toml` extension is not required, but the format must match what is found in the included `config.toml`. 

### Setting up the Config File

The configuration file allows users to easily define what experiments should be run in aa repeatable manner. Additionally, multiple experiments can be executed in a single program run if they are included in the configuration file. For each experiment, the user simply provides a top-level `[[experiment]]` tag containing various fields. An example experiment configuration is as follows:

```toml
[[experiment]]
experiment_name = "Schwefel_Benchmark_Example"
upper_bound = 100.0
lower_bound = -100.0
population_size = 30
problem_type = 1
dimension = 30
seed = 1
```

#### Experiment Config Fields

- ***experiment_name***

    This field provides the name for an individual experiment. Though not required, if included it will provide the name of the directory created to store experiment results.

- ***upper_bound and lower_bound***

    Defines the largest and smallest values (inclusive) that can be included in the experiment's population. Sensible defaults for each benchmark function are provided if bounds not included in the configuration file.

- ***population_size***

    Sets the population size for the experiment. Defaults to 30 if not included.

- ***problem_type***

    Numerical identifier for which standard benchmark function to use. Valid inputs are in range *[1, 10]*.

- ***dimension***

    Defines the number of dimensions to be used in experiment.

- ***seed***

    Allows users to force specific seed for population generation, enabling easily repeatable experimentation.



## Analysis Script