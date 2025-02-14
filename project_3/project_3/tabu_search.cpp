#include "tabu_search.h"
#include <algorithm>
#include <random>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <cmath>

TabuSearch::TabuSearch(const std::vector<std::vector<int>> &matrix, const Config &cfg)
    : distanceMatrix(matrix), config(cfg), bestCost(INT_MAX) {
    // Walidacja macierzy odległości
    if (distanceMatrix.empty() || distanceMatrix.size() != distanceMatrix[0].size()) {
        throw std::invalid_argument("Invalid distance matrix: must be square and non-empty.");
    }

    numCities = matrix.size();
    iterationsWithoutImprovement = 0;

    // Rozmiar listy tabu (maksymalna liczba aktywnych zakazów)
    tabuListSize = static_cast<int>(std::ceil(cfg.tabuListMultiplier * numCities));
    tabuList = std::vector<std::vector<int>>(numCities, std::vector<int>(numCities, 0));

    // Kadencja tabu
    tabuTenure = std::max(1, static_cast<int>(std::ceil(cfg.tabuTenureMultiplier * numCities)));
}

std::vector<int> TabuSearch::generateInitialSolution() {
    if (config.initialSolution == 1) { // Random
        std::vector<int> solution(numCities);
        std::iota(solution.begin(), solution.end(), 0);
        std::shuffle(solution.begin(), solution.end(), std::mt19937{std::random_device{}()});
        return solution;
    } else { // Nearest Neighbor from all starting cities
        std::vector<int> bestSolution;
        int bestCost = INT_MAX;

        for (int startCity = 0; startCity < numCities; ++startCity) {
            std::vector<int> solution;
            std::vector<bool> visited(numCities, false);
            int currentCity = startCity;
            solution.push_back(currentCity);
            visited[currentCity] = true;

            while (solution.size() < numCities) {
                int nearestCity = -1;
                int minDistance = INT_MAX;

                for (int i = 0; i < numCities; ++i) {
                    if (!visited[i] && distanceMatrix[currentCity][i] < minDistance) {
                        minDistance = distanceMatrix[currentCity][i];
                        nearestCity = i;
                    }
                }

                currentCity = nearestCity;
                visited[currentCity] = true;
                solution.push_back(currentCity);
            }

            // Calculate cost of this NN solution
            int cost = calculateCost(solution);
            if (cost < bestCost) {
                bestCost = cost;
                bestSolution = solution;
            }
        }

        return bestSolution;
    }
}

int TabuSearch::calculateCost(const std::vector<int> &solution) {
    int cost = 0;
    for (size_t i = 0; i < solution.size() - 1; ++i) {
        cost += distanceMatrix[solution[i]][solution[i + 1]];
    }
    cost += distanceMatrix[solution.back()][solution[0]]; // Return to start
    return cost;
}

void TabuSearch::swapCities(std::vector<int> &solution, int i, int j) {
    std::swap(solution[i], solution[j]);
}

bool TabuSearch::isTabu(int city1, int city2) const {
    return tabuList[city1][city2] > 0;
}

bool TabuSearch::aspirationCriteria(int currentCost) const {
    return currentCost < bestCost - std::max(1, aspirationCriterion);
}

bool TabuSearch::isAllowed(int city1, int city2, int currentCost) const {
    return !isTabu(city1, city2) || aspirationCriteria(currentCost);
}

void TabuSearch::updateTabuList(int city1, int city2) {
    // Jeśli liczba aktywnych zakazów przekracza tabuListSize, zmniejsz najstarsze
    int activeTabuCount = 0;
    for (const auto& row : tabuList) {
        for (int val : row) {
            if (val > 0) activeTabuCount++;
        }
    }

    if (activeTabuCount >= tabuListSize) {
        for (auto& row : tabuList) {
            for (auto& val : row) {
                if (val > 0) val--; // Dekrementacja najstarszych zakazów
            }
        }
    }

    // Ustaw nowy zakaz
    tabuList[city1][city2] = tabuTenure;
    tabuList[city2][city1] = tabuTenure; // Symmetric tabu tenure
}

void TabuSearch::decrementTabuList() {
    for (auto& row : tabuList) {
        for (auto& val : row) {
            if (val > 0) val--; // Zmniejsz tylko aktywne wartości
        }
    }
}

std::vector<int> TabuSearch::applyTwoOpt(const std::vector<int>& solution, int& bestNeighborCost) {
    std::vector<int> bestNeighbor = solution;
    bestNeighborCost = INT_MAX;

    int neighborsChecked = 0; // Licznik sprawdzonych rozwiązań
    bool aspirationMet = false; // Czy aspiracja została spełniona

    for (int i = 0; i < numCities - 1 && neighborsChecked < config.maxM; ++i) {
        for (int j = i + 1; j < numCities && neighborsChecked < config.maxM; ++j) {
            neighborsChecked++; // Licznik przeglądanych rozwiązań
            std::vector<int> neighbor = solution;
            std::reverse(neighbor.begin() + i, neighbor.begin() + j + 1);
            int cost = calculateCost(neighbor);

            if (isAllowed(solution[i], solution[j], cost) && cost < bestNeighborCost) {
                bestNeighbor = neighbor;
                bestNeighborCost = cost;
            }

            // Kryterium aspiracji plus
            if (config.aspirationPlus && cost >= config.aspirationCriterion) {
                aspirationMet = true;
            }

            // Jeśli aspiracja spełniona, przeglądaj dodatkowe `plus` sąsiadów
            if (config.aspirationPlus && aspirationMet && neighborsChecked >= config.minM) {
                neighborsChecked += config.plus; // Dodanie dodatkowych przeglądów
            }
        }
    }

    return bestNeighbor;
}

std::vector<int> TabuSearch::applyCitySwap(const std::vector<int>& solution, int& bestNeighborCost) {
    std::vector<int> bestNeighbor = solution;
    bestNeighborCost = INT_MAX;

    int neighborsChecked = 0; // Licznik sprawdzonych rozwiązań
    bool aspirationMet = false; // Czy aspiracja została spełniona

    for (int i = 0; i < numCities - 1 && neighborsChecked < config.maxM; ++i) {
        for (int j = i + 1; j < numCities && neighborsChecked < config.maxM; ++j) {
            neighborsChecked++; // Licznik przeglądanych rozwiązań
            std::vector<int> neighbor = solution;
            swapCities(neighbor, i, j);
            int cost = calculateCost(neighbor);

            if (isAllowed(solution[i], solution[j], cost) && cost < bestNeighborCost) {
                bestNeighbor = neighbor;
                bestNeighborCost = cost;
            }

            // Kryterium aspiracji plus
            if (config.aspirationPlus && cost >= config.aspirationCriterion) {
                aspirationMet = true;
            }

            // Jeśli aspiracja spełniona, przeglądaj dodatkowe `plus` sąsiadów
            if (config.aspirationPlus && aspirationMet && neighborsChecked >= config.minM) {
                neighborsChecked += config.plus; // Dodanie dodatkowych przeglądów
            }
        }
    }

    return bestNeighbor;
}

struct VectorHash {
    std::size_t operator()(const std::vector<int>& vec) const {
        std::size_t seed = vec.size();
        for (auto& i : vec) {
            seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

std::vector<int> TabuSearch::solve() {
    auto start = std::chrono::high_resolution_clock::now();
    auto bestSolution = generateInitialSolution();
    auto currentSolution = bestSolution;
    bestCost = calculateCost(bestSolution);
    std::unordered_set<std::vector<int>, VectorHash> solutionHistory; // Historia rozwiązań

    iterations = 0;

    while (iterationsWithoutImprovement < config.maxIterationsWithoutImprovement) {
        auto now = std::chrono::high_resolution_clock::now();
        double elapsedTime = std::chrono::duration<double, std::milli>(now - start).count();
        if (elapsedTime > config.terminationTime) {
            std::cerr << "Termination due to time limit reached.\n";
            break;
        }

        int bestNeighborCost = INT_MAX;
        std::vector<int> bestNeighbor;

        // Wybór metody przeglądu sąsiedztwa
        if (config.swapWay == 0) { // Apply 2-opt
            bestNeighbor = applyTwoOpt(currentSolution, bestNeighborCost);
        } else if (config.swapWay == 1) { // Apply city swap
            bestNeighbor = applyCitySwap(currentSolution, bestNeighborCost);
        }

        // Obsługa zdarzenia krytycznego
        if (bestNeighborCost == INT_MAX) {
            if (config.enableCriticalEventHandling) {
                std::cerr << "Critical event: No valid move found. Restarting with a random solution.\n";
                auto newSolution = generateInitialSolution();
                int newSolutionCost = calculateCost(newSolution);
                if (newSolutionCost < bestCost) {
                    bestCost = newSolutionCost;
                    bestSolution = newSolution;
                    currentSolution = newSolution;
                    iterationsWithoutImprovement = 0; // Reset stagnacji
                } else {
                    currentSolution = newSolution;
                    iterationsWithoutImprovement++; // Inkrementacja stagnacji
                }
            } else {
                std::cerr << "Critical event: No valid move found. Terminating algorithm.\n";
                break;
            }
            continue;
        }

        currentSolution = bestNeighbor;

        // Aktualizacja listy tabu
        for (size_t i = 0; i < numCities - 1; ++i) {
            updateTabuList(currentSolution[i], currentSolution[i + 1]);
        }
        updateTabuList(currentSolution.back(), currentSolution[0]);

        // Aktualizacja najlepszych wyników
        if (bestNeighborCost < bestCost) {
            bestCost = bestNeighborCost;
            bestSolution = currentSolution;
            iterationsWithoutImprovement = 0; // Reset liczby iteracji bez poprawy
            std::cout << "Improvement found! Best cost updated to: " << bestCost << std::endl;
        } else {
            iterationsWithoutImprovement++;
        }

        decrementTabuList(); // Dekrementacja listy tabu

        // Wskaźnik postępu
        if (config.showProgress) {
            std::cout << "Iteration: " << iterations
                      << ", Best Cost: " << bestCost
                      << ", Iterations Without Improvement: " << iterationsWithoutImprovement
                      << std::endl;
        }

        iterations++;
    }

    return bestSolution;
}
