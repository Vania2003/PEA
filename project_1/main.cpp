#include <iostream>
#include <chrono>
#include "config.h"
#include "tsp_utils.h"
#include "bruteforce.h"
#include "nearestneighbor.h"
#include "randomalgorithm.h"

using namespace std;
using namespace chrono;

int main() {
    try {
        string configFile = "./config.txt";
        Config config = readConfig(configFile);

        // Wczytanie danych
        int numCities;
        vector<vector<int>> matrix;
        int optimalValue;
        vector<int> optimalPath;
        string pathFromFile = TSPUtils::readMatrix(config.dataFile, numCities, matrix, optimalValue, optimalPath);

        // Zmienne do zbierania wyników
        vector<double> executionTimes;
        vector<int> bestPath;
        int minCost = INT_MAX;

        auto startOverall = chrono::high_resolution_clock::now();

        for (int i = 0; i < config.repeats; ++i) {
            auto start = chrono::high_resolution_clock::now();

            pair<vector<int>, int> result;

            if (config.algorithmType == "BF") {
                result = solveTSPBruteForce(matrix, config.showProgress, startOverall, config.timeLimit, optimalValue);
            } else if (config.algorithmType == "NN") {
                if (config.runAllStartVertices) {
                    result = solveTSPNearestNeighborAllStartVertices(matrix, config.showProgress, config.timeLimit);
                } else {
                    result = solveTSPNearestNeighbor(matrix, config.showProgress, startOverall, config.timeLimit, config.startVertex);
                }
            } else if (config.algorithmType == "random") {
                result = solveTSPRandom(matrix, config.showProgress, startOverall, config.timeLimit, optimalValue);
            }

            auto end = chrono::high_resolution_clock::now();
            double execTime = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1e6; // W sekundach

            executionTimes.push_back(execTime);

            if (result.second < minCost) {
                minCost = result.second;
                bestPath = result.first;
            }
        }

        auto endOverall = chrono::high_resolution_clock::now();
        double durationOverall = chrono::duration_cast<chrono::milliseconds>(endOverall - startOverall).count();

        TSPUtils::displayResults(config.dataFile, minCost, bestPath, durationOverall, numCities, true, optimalValue);
        TSPUtils::saveResults(config.outputFile, config.dataFile, bestPath, minCost, executionTimes, optimalValue, optimalPath, pathFromFile);

    } catch (const runtime_error &e) {
        cerr << "Błąd: " << e.what() << endl;
        return 1;
    }

    cout << "Nacisnij Enter, aby zakonczyc program..." << endl;
    cin.get();

    return 0;
}
