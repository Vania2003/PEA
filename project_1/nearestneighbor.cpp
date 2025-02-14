#include "nearestneighbor.h"
#include "tsp_utils.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <limits>

using namespace std;
using namespace chrono;

pair<vector<int>, int> solveTSPBranch(
        const vector<vector<int>> &matrix,
        vector<int> &unvisited,
        vector<int> &currentTour,
        int currentCost,
        const time_point<high_resolution_clock> &startOverall,
        int timeLimit,
        bool showProgress
) {
    if (unvisited.empty()) {
        // Gdy wszystkie wierzchołki zostały odwiedzone, wracamy do początku trasy
        currentTour.push_back(currentTour[0]);
        currentCost += matrix[currentTour[currentTour.size() - 2]][currentTour[0]];
        return {currentTour, currentCost};
    }

    int bestCost = numeric_limits<int>::max();
    vector<int> bestTour;

    int currentCity = currentTour.back();
    vector<int> candidates;
    int minDistance = numeric_limits<int>::max();

    for (int city : unvisited) {
        int distance = matrix[currentCity][city];
        if (distance < minDistance) {
            candidates.clear();
            candidates.push_back(city);
            minDistance = distance;
        } else if (distance == minDistance) {
            candidates.push_back(city);
        }
    }

    // Sprawdzamy każdą możliwą trasę
    for (int candidate : candidates) {
        vector<int> newTour = currentTour;
        newTour.push_back(candidate);

        vector<int> newUnvisited = unvisited;
        newUnvisited.erase(find(newUnvisited.begin(), newUnvisited.end(), candidate));

        // Obliczamy koszty w sposób rekurencyjny, rozgałęziając trasę
        auto [tour, cost] = solveTSPBranch(matrix, newUnvisited, newTour, currentCost + matrix[currentCity][candidate], startOverall, timeLimit, showProgress);
        if (cost < bestCost) {
            bestCost = cost;
            bestTour = tour;
        }

        // Sprawdzenie limitu czasu
        if (timeLimit > 0) {
            auto now = high_resolution_clock::now();
            double elapsedTime = duration_cast<seconds>(now - startOverall).count();
            if (elapsedTime >= timeLimit) {
                cout << "Przekroczono limit czasu: " << timeLimit << " s. Przerywamy." << endl;
                return {bestTour, bestCost};
            }
        }
    }

    return {bestTour, bestCost};
}

pair<vector<int>, int> solveTSPNearestNeighbor(
        const vector<vector<int>> &matrix,
        bool showProgress,
        const time_point<high_resolution_clock> &startOverall,
        int timeLimit,
        int startVertex
) {
    int numCities = matrix.size();
    if (numCities == 0) throw runtime_error("Macierz nie zawiera miast.");
    if (startVertex >= numCities) throw runtime_error("Wierzchołek początkowy wykracza poza zakres miast.");

    vector<int> unvisited;
    for (int i = 0; i < numCities; ++i) {
        if (i != startVertex) unvisited.push_back(i);
    }

    vector<int> tour = {startVertex};
    auto [bestTour, bestCost] = solveTSPBranch(matrix, unvisited, tour, 0, startOverall, timeLimit, showProgress);

    return {bestTour, bestCost};
}

pair<vector<int>, int> solveTSPNearestNeighborAllStartVertices(const vector<vector<int>> &matrix, bool showProgress, int timeLimit) {
    int numCities = matrix.size();
    if (numCities == 0) throw runtime_error("Macierz nie zawiera miast.");

    int minCost = INT_MAX;
    vector<int> bestPath;

    for (int startVertex = 0; startVertex < numCities; ++startVertex) {
        auto startOverall = high_resolution_clock::now();

        // Wywołanie oryginalnej funkcji dla każdego wierzchołka
        auto result = solveTSPNearestNeighbor(matrix, showProgress, startOverall, timeLimit, startVertex);

        if (result.second < minCost) {
            minCost = result.second;
            bestPath = result.first;
        }
    }

    return {bestPath, minCost};
}
