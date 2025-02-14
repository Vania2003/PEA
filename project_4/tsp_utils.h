// Plik: tsp_utils.h
#ifndef TSP_UTILS_H
#define TSP_UTILS_H

#include <vector>
#include <string>
#include "config.h"

namespace TSPUtils {
    Config readConfig(const std::string &filename);
    void readMatrix(const std::string &filename, int &numCities, std::vector<std::vector<int>> &matrix, int &optimalValue);
    int calculatePathCost(const std::vector<int> &path, const std::vector<std::vector<int>> &matrix);
    void displayResults(const std::string &dataFile, int minCost, const std::vector<int> &bestPath,
                        double totalExecTime, int numCities, bool showPath, int bestOverallCost, int repeats);
    void saveResults(const std::string &outputFile, const std::string &dataFile, const std::vector<int> &bestPath,
                     int minCost, const std::vector<double> &executionTimes, int bestOverallCost,
                     const std::vector<int> &optimalPath, int repeats);
}

#endif // TSP_UTILS_H