#include <stdio.h>
#include <iostream>
#include "Experiment.h"


int main() {
    Experiment exp;
    return exp.runExperiment("config.toml");
}