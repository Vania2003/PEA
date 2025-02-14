#include "tsp_utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <numeric>

using namespace std;

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
                else if (key == "aspiration_criterion") config.aspirationCriterion = std::stoi(value);
                else if (key == "maxIterationsWithoutImprovement") config.maxIterationsWithoutImprovement = std::stoi(value);
                else if (key == "initial_solution") config.initialSolution = std::stoi(value);
                else if (key == "termination_time") config.terminationTime = std::stoi(value);
                else if (key == "tabu_list_multiplier") config.tabuListMultiplier = std::stod(value);
                else if (key == "tabu_tenure_multiplier") config.tabuTenureMultiplier = std::stod(value);
                else if (key == "aspiration_plus") config.aspirationPlus = (value == "true" || value == "1");
                else if (key == "enable_critical_event_handling");
                else if (key == "swap_way") config.swapWay = std::stoi(value);
                else if (key == "minM") config.minM = std::stoi(value);
                else if (key == "maxM") config.maxM = std::stoi(value);
                else if (key == "plus") config.plus = std::stoi(value);
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

    int calculatePathCost(const vector<int> &path, const vector<vector<int>> &matrix) {
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

    void saveResults(const string &outputFile, const string &dataFile, const vector<int> &bestPath,
                    int minCost, const vector<double> &executionTimes, int bestOverallCost,
                    const vector<int> &optimalPath, int repeats) {
        ofstream file(outputFile);
        if (!file.is_open()) {
            throw runtime_error("Nie można otworzyć pliku do zapisu wyników: " + outputFile);
        }

        double totalExecTime = accumulate(executionTimes.begin(), executionTimes.end(), 0.0);
        double avgExecTime = totalExecTime / repeats;

        file << "Nazwa instancji\tWynik optymalny\tSciezka optymalna\tMinimalny koszt\tBlad bezwzgledny\tBlad wzgledny (%)\tCzas calkowity (ms)\tCzas sredni (ms)" << endl;
        file << dataFile << "\t" << bestOverallCost << "\t";

        for (int city : optimalPath) file << city << " ";
        file << "\t" << minCost << "\t";

        if (bestOverallCost > 0 && minCost != INT_MAX) {
            int absoluteError = abs(minCost - bestOverallCost);
            double relativeError = 100.0 * static_cast<double>(absoluteError) / bestOverallCost;
            file << absoluteError << "\t" << fixed << setprecision(4) << relativeError << "\t";
        } else {
            file << "N/A\tN/A\t";
        }

        file << fixed << setprecision(4) << totalExecTime << "\t" << avgExecTime << endl;
        file.close();
    }
}