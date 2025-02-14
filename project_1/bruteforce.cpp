#include "bruteforce.h"
#include "tsp_utils.h"
#include <numeric>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace chrono;

pair<vector<int>, int> solveTSPBruteForce(const vector<vector<int>> &matrix, bool showProgress, const time_point<high_resolution_clock> &startOverall, int timeLimit, int optimalValue) {
    int numCities = matrix.size();
    if (numCities == 0) throw runtime_error("Macierz nie zawiera miast.");

    vector<int> cities(numCities - 1);
    iota(cities.begin(), cities.end(), 1);

    int minCost = INT_MAX;
    vector<int> bestPath;
    int currentPermutation = 0;

    do {
        // Sprawdzenie limitu czasu na początku każdej iteracji
        auto now = high_resolution_clock::now();
        double elapsedTime = duration_cast<seconds>(now - startOverall).count();
        if (timeLimit > 0 && elapsedTime >= timeLimit) {
            break;
        }

        // Tworzymy pełną ścieżkę, zaczynając od wierzchołka 0
        vector<int> path = {0};
        path.insert(path.end(), cities.begin(), cities.end());

        int currentCost = TSPUtils::calculatePathCost(path, matrix);
        if (currentCost < minCost) {
            minCost = currentCost;
            bestPath = path;
        }
        currentPermutation++;

        if (showProgress) {
            double execTime = duration_cast<duration<double>>(now - startOverall).count();
            cout << currentPermutation << " - " << currentCost << " - " << fixed << setprecision(6) << execTime << endl;
        }

    } while (next_permutation(cities.begin(), cities.end()));

    return {bestPath, minCost};
}
