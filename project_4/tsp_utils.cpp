#include "tsp_utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <stdexcept>

namespace TSPUtils {

    Config readConfig(const std::string &filename) {
        Config config;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku konfiguracyjnego: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            size_t separatorPos = line.find('=');
            if (separatorPos != std::string::npos) {
                std::string key = line.substr(0, separatorPos);
                std::string value = line.substr(separatorPos + 1);

                if (key == "data_file") config.dataFile = value;
                else if (key == "output_file") config.outputFile = value;
                else if (key == "repeats") config.repeats = std::stoi(value);
                else if (key == "show_progress") config.showProgress = (value == "true" || value == "1");
                else if (key == "population_size") config.populationSize = std::stoi(value);
                else if (key == "max_generations") config.maxGenerations = std::stoi(value);
                else if (key == "mutation_probability") config.mutationProbability = std::stod(value);
                else if (key == "selectionMethod") config.selectionMethod = value;
                else if (key == "crossoverMethod") config.crossoverMethod = value;
                else if (key == "crossover_probability") config.crossoverProbability = std::stod(value);
                else if (key == "elitePercentage") config.elitePercentage = std::stod(value);
                else if (key == "timeout") config.timeout = std::stoi(value); // Oczekuje czasu w sekundach
            }
        }
        file.close();
        return config;
    }



    void readMatrix(const std::string &filename, int &numCities, std::vector<std::vector<int>> &matrix, int &optimalValue) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + filename);
        }

        if (!(file >> numCities) || numCities <= 0) {
            throw std::runtime_error("Błąd: Nieprawidłowa liczba miast w pliku: " + filename);
        }

        matrix.resize(numCities, std::vector<int>(numCities));
        for (int i = 0; i < numCities; ++i) {
            for (int j = 0; j < numCities; ++j) {
                if (!(file >> matrix[i][j])) {
                    throw std::runtime_error("Błąd podczas wczytywania macierzy odległości: " + filename);
                }
                if (matrix[i][j] == -1) matrix[i][j] = (i == j) ? 0 : INT_MAX;
            }
        }

        if (!(file >> optimalValue) || optimalValue < 0) {
            throw std::runtime_error("Błąd: Nieprawidłowa wartość optymalna w pliku: " + filename);
        }

        file.close();
    }

    int calculatePathCost(const std::vector<int> &path, const std::vector<std::vector<int>> &matrix) {
        int cost = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int city1 = path[i];
            int city2 = path[i + 1];
            int currentCost = matrix[city1][city2];
            if (currentCost == INT_MAX) return INT_MAX;
            cost += currentCost;
        }

        int returnCost = matrix[path.back()][path[0]];
        if (returnCost == INT_MAX) return INT_MAX;
        cost += returnCost;

        return cost;
    }

    void displayResults(const std::string &dataFile, int minCost, const std::vector<int> &bestPath,
                        double totalExecTime, int numCities, bool showPath, int bestOverallCost, int repeats) {
        double avgExecTime = totalExecTime / repeats;

        std::cout << std::fixed << std::setprecision(4);
        std::cout << "-------------------------------------" << std::endl;
        std::cout << "Nazwa pliku z danymi: " << dataFile << std::endl;
        std::cout << "Minimalny koszt: " << minCost << std::endl;

        if (bestOverallCost > 0 && minCost != INT_MAX) {
            int absoluteError = abs(minCost - bestOverallCost);
            double relativeError = 100.0 * static_cast<double>(absoluteError) / bestOverallCost;
            std::cout << "Blad bezwzgledny: " << absoluteError << std::endl;
            std::cout << "Blad wzgledny: " << relativeError << "%" << std::endl;
        }

        std::cout << "Czas calkowity: " << totalExecTime << " ms" << std::endl;
        std::cout << "Czas sredni: " << avgExecTime << " ms" << std::endl;

        if (showPath && numCities < 16) {
            std::cout << "Sciezka dla rozwiazania: ";
            for (int city : bestPath) {
                std::cout << city << " -> ";
            }
            std::cout << bestPath[0] << std::endl;
        }

        std::cout << "-------------------------------------" << std::endl;
    }

    void saveResults(const std::string &outputFile, const std::string &dataFile, const std::vector<int> &bestPath,
                     int minCost, const std::vector<double> &executionTimes, int bestOverallCost,
                     const std::vector<int> &optimalPath, int repeats) {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku do zapisu wyników: " + outputFile);
        }

        double totalExecTime = std::accumulate(executionTimes.begin(), executionTimes.end(), 0.0);
        double avgExecTime = totalExecTime / repeats;

        file << "Nazwa instancji\tWynik optymalny\tSciezka optymalna\tMinimalny koszt\tBlad bezwzgledny\tBlad wzgledny (%)\tCzas calkowity (ms)\tCzas sredni (ms)" << std::endl;
        file << dataFile << "\t" << bestOverallCost << "\t";

        for (int city : optimalPath) file << city << " ";
        file << "\t" << minCost << "\t";

        if (bestOverallCost > 0 && minCost != INT_MAX) {
            int absoluteError = abs(minCost - bestOverallCost);
            double relativeError = 100.0 * static_cast<double>(absoluteError) / bestOverallCost;
            file << absoluteError << "\t" << std::fixed << std::setprecision(4) << relativeError << "\t";
        } else {
            file << "N/A\tN/A\t";
        }

        file << std::fixed << std::setprecision(4) << totalExecTime << "\t" << avgExecTime << std::endl;
        file.close();
    }
}
