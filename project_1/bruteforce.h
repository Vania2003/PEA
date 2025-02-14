#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include <vector>
#include <utility>
#include <chrono>

std::pair<std::vector<int>, int> solveTSPBruteForce(
        const std::vector<std::vector<int>> &matrix,
        bool showProgress,
        const std::chrono::time_point<std::chrono::high_resolution_clock> &startOverall,
        int timeLimit,
        int optimalValue);

#endif // BRUTEFORCE_H
