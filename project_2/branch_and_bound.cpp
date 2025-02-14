#include "branch_and_bound.h"
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

std::pair<std::vector<int>, int> solveBFS(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit) {
    BranchAndBoundSolver solver(matrix, showProgress);
    return solver.solveBFS();
}

std::pair<std::vector<int>, int> solveDFS(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit) {
    BranchAndBoundSolver solver(matrix, showProgress);
    return solver.solveDFS();
}

std::pair<std::vector<int>, int> solveLowestCost(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit) {
    BranchAndBoundSolver solver(matrix, showProgress);
    return solver.solveLowestCost();
}

BranchAndBoundSolver::BranchAndBoundSolver(const vector<vector<int>> &matrix, bool showProgress)
        : matrix(matrix), numCities(matrix.size()), showProgress(showProgress) {}

int BranchAndBoundSolver::calculateCost(const vector<int> &path) {
    int cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        cost += matrix[path[i]][path[i + 1]];
    }
    return cost;
}

int BranchAndBoundSolver::bound(const Node &node) {
    int boundCost = node.cost;
    set<int> remainingCities;
    for (int i = 0; i < numCities; ++i) {
        if (find(node.path.begin(), node.path.end(), i) == node.path.end()) {
            remainingCities.insert(i);
        }
    }

    // Dodajemy minimalny koszt z miast, które nie były jeszcze odwiedzone
    for (int city : remainingCities) {
        int minEdgeCost = INT_MAX;
        for (int nextCity : remainingCities) {
            if (city != nextCity) {
                minEdgeCost = min(minEdgeCost, matrix[city][nextCity]);
            }
        }
        if (minEdgeCost != INT_MAX) {
            boundCost += minEdgeCost;
        }
    }

    return boundCost;
}

// Implementacja metody przeszukiwania BFS z dodanym wskaźnikiem postępu
pair<vector<int>, int> BranchAndBoundSolver::solveBFS() {
    queue<Node> q;
    q.push(Node({0}, 0, 0));

    int minCost = INT_MAX;
    vector<int> bestPath;

    int iteration = 0;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        if (current.level == numCities - 1) {
            // Obliczamy całkowity koszt cyklu
            int totalCost = current.cost + matrix[current.path.back()][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = current.path;
                bestPath.push_back(0);
            }
            continue;
        }

        for (int i = 0; i < numCities; ++i) {
            if (find(current.path.begin(), current.path.end(), i) == current.path.end()) {
                vector<int> newPath = current.path;
                newPath.push_back(i);

                int newCost = current.cost + matrix[current.path.back()][i];

                Node newNode(newPath, newCost, current.level + 1);
                int lowerBound = bound(newNode);

                if (lowerBound < minCost) {
                    q.push(newNode);
                }
            }
        }

        // Wyświetlanie postępu
        if (iteration % 1000 == 0 && showProgress) {
            cout << "BFS (z ograniczeniem): Iteracja " << iteration << ", biezacy koszt: " << current.cost << endl;
        }
        iteration++;
    }

    return {bestPath, minCost};
}


// Implementacja metody przeszukiwania DFS z dodanym wskaźnikiem postępu
pair<vector<int>, int> BranchAndBoundSolver::solveDFS() {
    stack<Node> s;
    s.push(Node({0}, 0, 0));

    int minCost = INT_MAX;
    vector<int> bestPath;

    int iteration = 0;

    while (!s.empty()) {
        Node current = s.top();
        s.pop();

        if (current.level == numCities - 1) {
            int totalCost = current.cost + matrix[current.path.back()][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = current.path;
                bestPath.push_back(0); // Powrót do miasta początkowego
            }
            continue;
        }

        // Dodanie przycinania dla DFS
        for (int i = 0; i < numCities; ++i) {
            if (find(current.path.begin(), current.path.end(), i) == current.path.end()) {
                int newCost = current.cost + matrix[current.path.back()][i];
                if (newCost >= minCost) {
                    // Przycinanie nieopłacalnych ścieżek
                    continue;
                }
                vector<int> newPath = current.path;
                newPath.push_back(i);
                s.push(Node(newPath, newCost, current.level + 1));
            }
        }

        // Wyświetlanie postępu
        if (iteration % 1000 == 0 && showProgress) {
            cout << "DFS: Iteracja " << iteration << ", biezacy koszt: " << current.cost << endl;
        }
        iteration++;
    }

    return {bestPath, minCost};
}

// Implementacja metody przeszukiwania Lowest Cost z dodanym wskaźnikiem postępu
pair<vector<int>, int> BranchAndBoundSolver::solveLowestCost() {
    int n = matrix.size();
    int bestCost = numeric_limits<int>::max();
    vector<int> bestPath;

    auto compare = [](const Node &a, const Node &b) {
        return a.cost > b.cost;
    };
    priority_queue<Node, vector<Node>, decltype(compare)> pq(compare);
    pq.push(Node({0}, 0, 0));

    int iteration = 0;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.level == n - 1) {
            int totalCost = current.cost + matrix[current.path.back()][0];
            if (totalCost < bestCost) {
                bestCost = totalCost;
                bestPath = current.path;
                bestPath.push_back(0);
            }
            continue;
        }

        for (int i = 1; i < n; ++i) {
            if (find(current.path.begin(), current.path.end(), i) == current.path.end()) {
                int newCost = current.cost + matrix[current.path.back()][i];
                if (newCost < bestCost) {
                    vector<int> newPath = current.path;
                    newPath.push_back(i);
                    pq.push(Node(newPath, newCost, current.level + 1));
                }
            }
        }

        // Wyświetlanie postępu
        if (iteration % 1000 == 0 && showProgress) {
            cout << "Lowest Cost: Iteracja " << iteration << ", biezacy koszt: " << current.cost << endl;
        }
        iteration++;
    }

    return {bestPath, bestCost};
}
