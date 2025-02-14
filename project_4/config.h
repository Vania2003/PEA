// Plik: config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct Config {
    std::string dataFile;
    std::string outputFile;
    int repeats;
    bool showProgress;
    int populationSize;
    int maxGenerations;
    double mutationProbability;
    int eliteCount;
    std::string selectionMethod;
    std::string crossoverMethod;
    double crossoverProbability;
    int timeout; // W sekundach
    double elitePercentage;
};


#endif // CONFIG_H