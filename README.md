# Numerical Optimization Benchmarks

## Overview

This repository contains a benchmarking framework for a selection of standard benchmark functions. The program pseudo-randomly initializes a population and evaluates fitness for a selected problem. Inputs and problem are configurable by the user through a [config.toml](https://github.com/Ameb8/numerical-optimization-benchmarks/blob/master/config.toml) file. Additionally, this repository includes a script for calculating statistics and generating visualizations based off benchmarking results.

### Repository Structure

- ***docs***

    Contains project documentation, notably, a benchmark evaluation.

- ***example_configs***

    Contains additional example input config files for the project. These can be passed to the program by path to execute example benchmark experiments.

- ***include***

    Contains header files for the C++ benchmark program.

- ***results***

    Contains results of benchmark experiments. For each successfully-run benchmark, the program will generate a subdirectory containing all benchmark results.

- ***scripts***

    Contains Python source code.

- ***src***

    Contains C++ source files.

### Root Directory Files

In addition to subdirectories, this repository's root contains various important project files.

- ***config.toml***

Default benchmark configuration when no argument is passed.

- ***Makefile***

Defines compilation process for C++ Benchmark Program.

- ***pyproject.toml***

Configuration file for Python module. Allows users to easily run python code, ensuring their system has correct dependencies and Python interpreter.

- ***README.md***

This file, detailing how to use and run this program.

- ***requirements.txt***

Allows users to quickly install python dependencies with correct versions. This is, however, not the recommended method to run this program. Instructions for running this program with `requirements.txt` are not included in project documentation.

- ***.gitignore***

Prevents certain files and directories from being tracked in this repository. For example, the `build` and `bin` directories generated during program execution to store C++ build artifacts.

### Source Code

This source code of this project is separated into 2 distinct locations. The C++ Benchmarking program generates pseudo-random inputs, evaluates standard benchmark functions, and outputs results and evaluation times for each experiment. `src` contains the C++ files, while `include` contains source files. The `External` subdirectory in both locations includes C++ and header files from external sources. 

This project also contains a Python program which loads and validates benchmark configuration, compiles and executes the C++ program with said config, and produces plots and a document with experiment results. All python source code lives within `scripts/run_benchmark`.


## Project Requirements

This program was designed to run on MacOS and Linux systems. As such, attempting to run on a Windows device may cause errors, even if the WSL Linux-subsystem is used. Additionally, the Makefile provided utilizes g++ to compile the program for C++20, thus, GCC 11+ must be installed on the user's system. Furthermore, the `make` command is programmatically executed, meaning that make must be installed and available in the system's `PATH`. The C++ code does rely on some external dependencies, however, their source code has been included in this repository at [src/External](https://github.com/Ameb8/numerical-optimization-benchmarks/tree/master/src/External) and [include/External](https://github.com/Ameb8/numerical-optimization-benchmarks/tree/master/include/External). Thus, external C++ dependencies require no additional set up on the users part.

The system must have Python 3.11 or newer in order to run the program. Additionally, the user must have the *pip* installed in order to download dependencies and configure the project. Furthermore, it is highly recommended to avoid dependency version conflicts that a virtual environment manager such as *venv*, *virtualenv*, or *conda* is used to run this program. Although Python dependencies do not need to be installed individually, an exact list of required packages can be found [here](https://github.com/Ameb8/numerical-optimization-benchmarks/blob/master/pyproject.toml). 


## Running Benchmarks

### Compiling the Program

While this project automatically handles compilation of the *cpp* benchmark program, Setup must be performed before benchmarks can be run. This guide will assume a *venv* is being used, but this can be swapped for a different virtual environment if desired. Steps needed to setup project are as follows:

```bash
cd numerical-optimization-benchmarks
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip setuptools wheel
pip install .
```

The first step is to navigate to the root directory of this project, containing this README.md file. Next, a virtual environment is created. In this case, the environment is located in the project root and named `.venv`, but exact location and name does not matter as long as it is referenced consistently and holds a clean environment. Next, the virtual environment is activated. The following `pip install` ensures your environment's pip, setuptools, and wheel are up to date. Next, `pip install .` can be ran to install the Python source code as a package in the active environment. While the *CPP* code remains non-compiled, this will be handled by the program at runtime and is not recommended to be done manually during normal use.

#### Compiling C Plus Plus Code Manually

If the user desirees to compile the benchmark program manually, the included [Makefile](https://github.com/Ameb8/numerical-optimization-benchmarks/blob/master/Makefile) allows for a simple process. To compile, the user must navigate to the root directory of this repository. It is then recommended (but not always required) that the user runs `make clean`. This should result in output something like `rm -rf build bin`. Then, the user simply runs the `make` command. If successful, the output should look something like:

```bash
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/Experiment.cpp -o build/Experiment.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/TestRunner.cpp -o build/TestRunner.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/External/mt.cpp -o build/External/mt.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/Population/Population.cpp -o build/Population/Population.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/BenchmarkRunner.cpp -o build/BenchmarkRunner.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/ProblemFactory.cpp -o build/ProblemFactory.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -c src/main.cpp -o build/main.o
g++ -std=c++20 -Wall -Wextra -Iinclude -O3 -o bin/benchmark build/Experiment.o build/TestRunner.o build/Utils/mt.o build/Population/Population.o build/Config.o build/ProblemFactory.o build/main.o
```

Additionally, the user can inspect the `bin/` directory to see if it contains an executable program named `benchmark` in order to validate build success. If this file appears, the program is ready for execution.

### Executing the Program

Once project setup has been completed, you are ready to begin running benchmarks. From the project's root directory, you simply need to run `numerical-benchmarks` in the same python environment you installed the project in. While the project will attempt to execute regardless of the directory it is ran from, the project is not setup to work from any cwd in its current state. Running the program like this also relies on a valid `config.toml` file existing in the project root, like the one included in the repository. 

Alternatively, a path relative to project root to a valid config file con optionally be passed. It is not required that this file is named `config.toml`, just that the file data is formatted correctly and that the path is relative to the project root (where the command must be ran from). For example, to run the `large_population.toml` benchmark config file found in the `example_configs` directory, you would run:

```bash
numerical-benchmarks example_configs/large_population.toml
```

If the program runs successfully, results will be written to the `results/<benchmark_name>`, with the benchmark_name coming from the config file. More detailed information about the contents and structure of benchmark results can be found in the results section of this document


## Setting up the Benchmark Configs

The configuration file allows users to easily define what experiments should be run in a repeatable manner. Additionally, multiple experiments can be executed in a single program run if they are included in the configuration file. For each experiment, the user simply provides a top-level `[[experiment]]` tag containing various fields. An example experiment configuration is as follows:

```toml
[[experiment]]
experiment_name = "Schwefel_Benchmark_Example"
upper_bound = 100.0
lower_bound = -100.0
problem_type = 1
dimension = 30
seed = 1
```

The `[benchmark]` tag contains the population size used for experiments and the name of the benchmark run:

```toml
[benchmark]
benchmark_name = "Function_Benchmarks"
population_size = 30
```

### Benchmark Config Fields

- ***benchmark_name***

    Sets name of benchmark, which defines the name of the directory used to store benchmark results. Defaults to timestamp: *YYYYMMDD_HHMMSS*.

- ***population_size***

    Sets the population size for all experiments. Defaults to 30 if not included.


### Experiment Config Fields


- ***experiment_name***

    This field provides the name for an individual experiment. Though not required, if included it will provide the name of the directory created to store experiment results.

- ***upper_bound and lower_bound***

    Defines the largest and smallest values (inclusive) that can be included in the experiment's population. Sensible defaults for each benchmark function are provided if bounds not included in the configuration file.


- ***problem_type***

    Numerical identifier for which standard benchmark function to use. Valid inputs are in range *[1, 10]*.

- ***dimension***

    Defines the number of dimensions to be used in experiment.

- ***seed***

    Allows users to force specific seed for population generation, enabling easily repeatable experimentation.


## Benchmark Results 

### Result Location

Experiment results will show up in the `results` directory under the repository root. Each successful program run will produce a subdirectory named after benchmark.benchmark_name in the config file. This directory will contain all results from the experiment.

### Result Content

The program begins each run by reading and validating the config file. Because config values can be set as defaults, the program produces a `benchmark.json` file in the benchmarks subdirectory, containing the final config values for the benchmark. This file is treated as the definitive source of truth when running benchmarks. After the C++ benchmark program has read the file and conducted all experiments, it outputs `fitness.csv` and `time.csv` in the benchmarks results directory. These files contain the raw fitness values and total wall-clock execution time for each experiment. The python program then reads this data and produces various graphs in the `plots` subdirectory. Additionally, the program constructs a ***LaTeX*** document in the `docs` subdirectory, with all plots as well as data tables included. The generated document is not properly formatted for an official report, it simply includes result analysis artifacts created by the program.



