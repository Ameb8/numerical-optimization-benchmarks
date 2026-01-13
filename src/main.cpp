#include <stdio.h>
#include "mt.h"

int testTwister() {
    MersenneTwister mt;
    mt.print();
    for(int i = 0; i < 5; i++)
        printf("%f\n", mt.random());
    return 0;
}


int main() {
    testTwister();

    // Read and initialize config file

    // Initialize RNG with config seed

    // generate Problem object with ProblemFactory

    // Create/initialize population

    // Evaluate problem with given population

    // Write to output file

    // Cleanup memory

    return 0;
}