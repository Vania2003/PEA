#include "randomalgorithm.h"
#include "tsp_utils.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

pair<vector<int>, int> solveTSPRandom(const vector<vector<int>> &matrix, bool showProgress,
                                      const time_point<high_resolution_clock> &startOverall, int timeLimit, int optimalValue) {
    int numCities = matrix.size();
    if (numCities == 0) throw runtime_error("Macierz nie zawiera miast.");

    int minCost = INT_MAX;
    vector<int> bestPath;

    random_device rd;
    mt19937 rng(rd());

    int currentPermutation = 0;

    while (true) {
        // Sprawdzenie limitu czasu na początku każdej iteracji
        if (timeLimit > 0) {
            auto now = high_resolution_clock::now();
            double elapsedTime = duration_cast<seconds>(now - startOverall).count();
            if (elapsedTime >= timeLimit) {
                cout << "Przekroczono limit czasu: " << timeLimit << " s. Przerywamy." << endl;
                break;
            }
        }

        // Inicjalizacja trasy zaczynajac od losowego wierzcholka
        vector<int> tour(numCities);
        iota(tour.begin(), tour.end(), 0);

        // Losowe przetasowanie wszystkich miast
        shuffle(tour.begin(), tour.end(), rng);

        // Dodajemy powrot do poczatkowego miasta, aby utworzyc cykl Hamiltona
        tour.push_back(tour[0]);

        // Obliczenie kosztu trasy
        int currentCost = TSPUtils::calculatePathCost(tour, matrix);
        if (currentCost < minCost) {
            minCost = currentCost;
            bestPath = tour;

            // Sprawdzenie, czy osiągnęliśmy optymalny wynik
            if (minCost == optimalValue) {
                break;
            }
        }

        // Zwiększenie liczby permutacji
        currentPermutation++;

        if (showProgress) {
            auto now = high_resolution_clock::now();
            double execTime = duration_cast<duration<double>>(now - startOverall).count();
            cout << "Iteracja: " << currentPermutation
                 << " - Koszt: " << currentCost << " - Czas: " << fixed << setprecision(6) << execTime << " s" << endl;
        }
    }

    return {bestPath, minCost};
}
