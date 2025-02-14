#ifndef TSP_UTILS_H
#define TSP_UTILS_H

#include <vector>
#include <string>

using namespace std;

class TSPUtils {
public:
    static string readMatrix(const string &filename, int &numCities, vector<vector<int>> &matrix, int &optimalValue, vector<int> &optimalPath);
    static int calculatePathCost(const std::vector<int> &path, const std::vector<std::vector<int>> &matrix);
    static void displayResults(const std::string &dataFile, int minCost, const std::vector<int> &bestPath, double execTime,
                               int numCities, bool showPath, int bestOverallCost);
    static void saveResults(const std::string &outputFile, const std::string &dataFile, const std::vector<int> &bestPath, int minCost,
                            const std::vector<double> &executionTimes, int bestOverallCost, const std::vector<int> &optimalPath, string &pathFromFile);
};

#endif // TSP_UTILS_H
