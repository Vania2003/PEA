// Plik: genetic_algorithm.h
#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>
#include <random>
#include "config.h"

class GeneticAlgorithm {
private:
    std::vector<std::vector<int>> distanceMatrix;
    Config config;
    std::vector<std::vector<int>> population;
    std::vector<int> fitness;
    std::mt19937 randomEngine;

    void initializePopulation();
    void calculateFitness();
    std::pair<int, int> tournamentSelection();
    std::pair<int, int> rouletteWheelSelection();
    std::vector<int> orderCrossover(const std::vector<int> &parent1, const std::vector<int> &parent2);
    std::vector<int> pmxCrossover(const std::vector<int> &parent1, const std::vector<int> &parent2);
    void reciprocalExchangeMutation(std::vector<int> &individual);
    void partialReplacement();
    int calculatePathCost(const std::vector<int> &path);

public:
    GeneticAlgorithm(const std::vector<std::vector<int>> &matrix, const Config &cfg);
    std::vector<int> solve();
};

#endif // GENETIC_ALGORITHM_H
