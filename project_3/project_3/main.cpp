// Updated main.cpp with extended functionality
#include "tsp_utils.h"
#include "tabu_search.h"
#include "config.h"
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
    try {
        // Wczytanie konfiguracji
        Config config = TSPUtils::readConfig("D:\\project_3\\project_3\\config.txt");

        // Wczytanie danych
        int numCities;
        std::vector<std::vector<int>> matrix;
        int optimalValue;

        TSPUtils::readMatrix(config.dataFile, numCities, matrix, optimalValue);

        // Powtórzenia algorytmu
        std::vector<double> executionTimes;
        std::vector<int> bestCosts;
        std::vector<std::vector<int>> bestPaths;

        for (int repeat = 0; repeat < config.repeats; ++repeat) {
            TabuSearch tabuSearch(matrix, config);

            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> bestPath = tabuSearch.solve();
            auto end = std::chrono::high_resolution_clock::now();

            double execTime = std::chrono::duration<double, std::milli>(end - start).count();
            int cost = TSPUtils::calculatePathCost(bestPath, matrix);

            executionTimes.push_back(execTime);
            bestCosts.push_back(cost);
            bestPaths.push_back(bestPath);
        }

        // Obliczenie wyników zbiorczych
        double avgTime = std::accumulate(executionTimes.begin(), executionTimes.end(), 0.0) / executionTimes.size();
        auto minCostIt = std::min_element(bestCosts.begin(), bestCosts.end());
        int bestOverallCost = *minCostIt;
        int bestIndex = std::distance(bestCosts.begin(), minCostIt);
        std::vector<int> bestOverallPath = bestPaths[bestIndex];

        double totalExecTime = std::accumulate(executionTimes.begin(), executionTimes.end(), 0.0);
        TSPUtils::displayResults(config.dataFile, bestOverallCost, bestOverallPath,
                                 totalExecTime, numCities, true, optimalValue, config.repeats);
        TSPUtils::saveResults(config.outputFile, config.dataFile, bestOverallPath,
                              bestOverallCost, executionTimes, optimalValue, bestOverallPath, config.repeats);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cin.get();
    return 0;
}
