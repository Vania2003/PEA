#ifndef RANDOMALGORITHM_H
#define RANDOMALGORITHM_H

#include <vector>
#include <utility>
#include <chrono>

std::pair<std::vector<int>, int> solveTSPRandom(
        const std::vector<std::vector<int>> &matrix, bool showProgress,
        const std::chrono::time_point<std::chrono::high_resolution_clock> &startOverall, int timeLimit, int optimalValue);

#endif // RANDOMALGORITHM_H
