#include "tsp_utils.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "limits"
#include <stdexcept>
#include <numeric>
#include <climits>

using namespace std;

string TSPUtils::readMatrix(const string &filename, int &numCities, vector<vector<int>> &matrix, int &optimalValue, vector<int> &optimalPath) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Nie można otworzyć pliku: " + filename);
    }

    if (!(file >> numCities) || numCities <= 0) {
        throw runtime_error("Błąd w odczycie liczby miast lub nieprawidłowa liczba miast w pliku: " + filename);
    }

    matrix.resize(numCities, vector<int>(numCities));
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if (!(file >> matrix[i][j])) {
                throw runtime_error("Błąd w odczycie macierzy odległości w pliku: " + filename);
            }
            if (matrix[i][j] == -1) {
                if (i == j) {
                    matrix[i][j] = 0; // Na przekątnej zamieniamy -1 na 0
                } else {
                    matrix[i][j] = INT_MAX; // Brak połączenia między różnymi miastami
                }
            }
        }
    }

    // Wczytywanie wartości optymalnej
    if (!(file >> optimalValue) || optimalValue < 0) {
        throw runtime_error("Błąd w odczycie wartości optymalnej lub nieprawidłowa wartość optymalna w pliku: " + filename);
    }

    optimalPath.clear();
    string pathString;
    file.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!getline(file, pathString) || pathString.empty()) {
        throw runtime_error("Błąd w odczycie ścieżki optymalnej z pliku: " + filename);
    }

    istringstream pathStream(pathString);
    int vertex;
    while (pathStream >> vertex) {
        optimalPath.push_back(vertex);
    }

    // Sprawdzenie, czy ścieżka optymalna została poprawnie wczytana
    if (optimalPath.empty()) {
        throw runtime_error("Błąd w odczycie ścieżki optymalnej z pliku: " + filename);
    }

    file.close();

    return pathString;
}

int TSPUtils::calculatePathCost(const vector<int> &path, const vector<vector<int>> &matrix) {
    int cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int city1 = path[i];
        int city2 = path[i + 1];
        int currentCost = matrix[city1][city2];
        if (currentCost == INT_MAX) {
            return INT_MAX;
        }
        cost += currentCost;
    }

    // Dodanie kosztu powrotu do miasta startowego
    int returnCost = matrix[path.back()][path[0]];
    if (returnCost == INT_MAX) {
        return INT_MAX;
    }
    cost += returnCost;

    return cost;
}

void TSPUtils::displayResults(const string &dataFile, int minCost, const vector<int> &bestPath, double execTime, int numCities, bool showPath, int bestOverallCost) {
    cout << fixed << setprecision(4);
    cout << "-------------------------------------" << endl;
    cout << "Nazwa pliku z danymi: " << dataFile << endl;

    // Wyświetlenie minimalnego kosztu
    cout << "Minimalny koszt: " << minCost << endl;

    // Obliczanie i wyświetlanie błędu bezwzględnego oraz względnego, jeśli jest to możliwe
    if (bestOverallCost > 0 && minCost != INT_MAX) {
        int absoluteError = abs(minCost - bestOverallCost);
        double relativeError = 100.0 * static_cast<double>(absoluteError) / bestOverallCost;
        cout << "Blad bezwzgledny: " << absoluteError << endl;
        cout << "Blad wzgledny: " << relativeError << "%" << endl;
    }

    // Wyświetlanie czasu realizacji w sekundach
    cout << "Czas realizacji: " << execTime / 1000.0 << " s" << endl;


    // Wyświetlanie ścieżki, ale tylko jeśli liczba miast jest rozsądna (np. < 16)
    if (showPath && numCities < 16) {
        cout << "Sciezka dla rozwiazania: ";
        for (int city : bestPath) {
            cout << city << " -> ";
        }
        cout << bestPath[0] << endl; // Powrót do miasta początkowego
    }

    cout << "-------------------------------------" << endl;
}

void TSPUtils::saveResults(const string &outputFile, const string &dataFile, const vector<int> &bestPath, int minCost,
                           const vector<double> &executionTimes, int bestOverallCost, const vector<int> &optimalPath, string &pathFromFile) {
    ofstream file(outputFile);
    if (!file.is_open()) {
        throw runtime_error("Nie mozna otworzyc pliku do zapisu wynikow: " + outputFile);
    }

    // Średni czas wykonania
    double avgTime = accumulate(executionTimes.begin(), executionTimes.end(), 0.0) / executionTimes.size();

    // Nagłówki kolumn
    file << "Nazwa instancji\tWynik optymalny\tSciezka optymalna\tMinimalny koszt\tBlad bezwzgledny\tBlad wzgledny (%)\tCzasy wykonania (s)\tSredni czas (s)" << endl;

    // Wynik optymalny
    file << dataFile << "\t" << bestOverallCost << "\t" << pathFromFile << "\t" << minCost << "\t";

    // Obliczanie błędu bezwzględnego i względnego
    if (bestOverallCost > 0 && minCost != INT_MAX) {
        int absoluteError = abs(minCost - bestOverallCost);
        double relativeError = 100.0 * static_cast<double>(absoluteError) / bestOverallCost;
        file << absoluteError << "\t" << fixed << setprecision(4) << relativeError << "\t";
    } else {
        file << "N/A\tN/A\t";
    }

    // Czasy wykonania każdej próby
    for (const auto &time : executionTimes) {
        file << fixed << setprecision(4) << time << " ";
    }

    file << "\t";
    file << fixed << setprecision(4) << avgTime << endl;

    file.close();
}
