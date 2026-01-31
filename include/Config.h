#ifndef CONFIG_H
#define CONFIG_H

#include <string>


typedef struct {
    std::string experimentName;
    int problemType;
    int dimensions;
    double lower;
    double upper;
    int seed;
    std::string optimizer;
    int maxIterations;
    double neighborDelta;
    int numNeighbors;
} ExperimentConfig;



#endif