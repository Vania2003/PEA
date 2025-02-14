#include <iostream>
#include <chrono>
#include "tsp_utils.h"
#include "branch_and_bound.h"

using namespace std;
using namespace chrono;

int main() {
    try {
        string configFile = "./config.txt";
        TSPUtils::Config config = TSPUtils::readConfig(configFile);

        // Wczytanie danych
        int numCities;
        vector<vector<int>> matrix;
        TSPUtils::readMatrix(config.dataFile, numCities, matrix);

        // Zmienne do zbierania wyników
        vector<double> executionTimes;
        vector<int> bestPath;
        int minCost = INT_MAX;

        auto startOverall = chrono::high_resolution_clock::now();

        for (int i = 0; i < config.repeats; ++i) {
            // Pomiar czasu dla algorytmu
            auto start = chrono::high_resolution_clock::now();

            pair<vector<int>, int> result;

            // Wybór algorytmu na podstawie konfiguracji
            if (config.algorithmType == "BFS") {
                result = solveBFS(matrix, config.showProgress, config.timeLimit);
            } else if (config.algorithmType == "DFS") {
                result = solveDFS(matrix, config.showProgress, config.timeLimit);
            } else if (config.algorithmType == "LowestCost") {
                result = solveLowestCost(matrix, config.showProgress, config.timeLimit);
            }

            // Pomiar czasu po zakończeniu algorytmu
            auto end = chrono::high_resolution_clock::now();
            double execTime = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1e6;
            executionTimes.push_back(execTime);

            // Aktualizacja najlepszego wyniku
            if (result.second < minCost) {
                minCost = result.second;
                bestPath = result.first;
            }
        }

        // Pomiar całkowitego czasu działania wszystkich iteracji
        auto endOverall = chrono::high_resolution_clock::now();
        double durationOverall = chrono::duration_cast<chrono::milliseconds>(endOverall - startOverall).count();

        // Wyświetlanie wyników
        TSPUtils::displayResults(config.dataFile, minCost, bestPath, durationOverall);

        // Zapis wyników do pliku
        TSPUtils::saveResults(config.outputFile, config.dataFile, bestPath, minCost, executionTimes);
    } catch (const runtime_error &e) {
        cerr << "Błąd: " << e.what() << endl;
        return 1;
    }

    cout << "Nacisnij Enter, aby zakonczyc program..." << endl;
    cin.get();

    return 0;
}
