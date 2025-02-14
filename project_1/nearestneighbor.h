#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H

#include <vector>
#include <utility>
#include <chrono>

std::pair<std::vector<int>, int> solveTSPBranch(
        const std::vector<std::vector<int>> &matrix,
        std::vector<int> &unvisited,
        std::vector<int> &currentTour,
        int currentCost,
        const std::chrono::time_point<std::chrono::high_resolution_clock> &startOverall,
        int timeLimit,
        bool showProgress);

std::pair<std::vector<int>, int> solveTSPNearestNeighbor(
        const std::vector<std::vector<int>> &matrix,
        bool showProgress,
        const std::chrono::time_point<std::chrono::high_resolution_clock> &startOverall,
        int timeLimit,
        int startVertex);

std::pair<std::vector<int>, int> solveTSPNearestNeighborAllStartVertices(
        const std::vector<std::vector<int>> &matrix,
        bool showProgress,
        int timeLimit);

#endif // NEARESTNEIGHBOR_H
