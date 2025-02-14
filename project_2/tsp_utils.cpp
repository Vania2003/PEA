#include "tsp_utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <limits.h>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

TSPUtils::Config TSPUtils::readConfig(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Nie można otworzyć pliku konfiguracyjnego: " + filename);
    }

    Config config;
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string key, value;

        if (iss >> key) {
            key = trim(key);
            if (key == "dataFile" || key == "outputFile" || key == "algorithmType") {
                getline(iss, value, '=');
                getline(iss, value);
                value = trim(value);

                if (key == "dataFile") {
                    config.dataFile = value;
                } else if (key == "outputFile") {
                    config.outputFile = value;
                } else if (key == "algorithmType") {
                    config.algorithmType = value;
                }
            } else if (key == "repeats" || key == "timeLimit") {
                getline(iss, value, '=');
                getline(iss, value);
                value = trim(value);

                try {
                    if (key == "repeats") {
                        config.repeats = stoi(value);
                    } else if (key == "timeLimit") {
                        config.timeLimit = stoi(value);
                    }
                } catch (const invalid_argument &e) {
                    throw runtime_error("Nieprawidłowa wartość dla klucza: " + key);
                }
            } else if (key == "showProgress") {
                getline(iss, value, '=');
                getline(iss, value);
                value = trim(value);

                config.showProgress = (value == "true");
            }
        }
    }

    file.close();
    validateConfig(config);
    return config;
}

void TSPUtils::validateConfig(const Config &config) {
    if (config.dataFile.empty() || config.outputFile.empty()) {
        throw runtime_error("Brakująca ścieżka do pliku danych lub pliku wyjściowego.");
    }
    if (config.repeats <= 0) {
        throw runtime_error("Liczba powtórzeń musi być większa od zera.");
    }
    if (config.timeLimit < 0) {
        throw runtime_error("Limit czasu musi być większy lub równy zero.");
    }
    if (!isValidAlgorithm(config.algorithmType)) {
        throw runtime_error("Nieprawidłowy typ algorytmu: " + config.algorithmType);
    }
}

bool TSPUtils::isValidAlgorithm(const string &algorithmType) {
    return algorithmType == "BFS" || algorithmType == "DFS" || algorithmType == "LowestCost";
}

void TSPUtils::readMatrix(const string &filename, int &numCities, vector<vector<int>> &matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Nie można otworzyć pliku: " + filename);
    }

    if (!(file >> numCities) || numCities <= 0) {
        throw runtime_error("Blad w odczycie liczby miast lub nieprawidłowa liczba miast w pliku: " + filename);
    }

    matrix.resize(numCities, vector<int>(numCities));
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Blad w odczycie macierzy odleglosci w pliku: " + filename);
            }
            if (matrix[i][j] == -1) {
                if (i == j) {
                    matrix[i][j] = 0;
                } else {
                    matrix[i][j] = INT_MAX;
                }
            }
        }
    }

    file.close();
}

void TSPUtils::saveResults(const string &outputFile, const string &dataFile, const vector<int> &bestPath, int minCost, const vector<double> &executionTimes) {
    ofstream file(outputFile);
    if (!file.is_open()) {
        throw runtime_error("Nie można otworzyć pliku do zapisu wyników: " + outputFile);
    }

    double avgTime = accumulate(executionTimes.begin(), executionTimes.end(), 0.0) / executionTimes.size();

    file << "Nazwa instancji\tMinimalny koszt\tCzasy wykonania (s)\tSredni czas (s)" << endl;

    file << dataFile << "\t" << minCost << "\t";

    for (const auto &time : executionTimes) {
        file << fixed << setprecision(4) << time << " ";
    }
    file << "\t";

    file << fixed << setprecision(4) << avgTime << endl;

    file.close();
}

void TSPUtils::displayResults(const string &dataFile, int minCost, const vector<int> &bestPath, double execTime) {
    cout << fixed << setprecision(4);
    cout << "-------------------------------------" << endl;
    cout << "Nazwa pliku z danymi: " << dataFile << endl;

    cout << "Minimalny koszt: " << minCost << endl;

    cout << "Czas realizacji: " << execTime / 1000.0 << " s" << endl;

    // Wyświetlenie ścieżki
    if (bestPath.size() < 16) {
        cout << "Sciezka rozwiazania: ";
        for (int city : bestPath) {
            cout << city << " -> ";
        }
        cout << bestPath[0] << endl;
    }

    cout << "-------------------------------------" << endl;
}
