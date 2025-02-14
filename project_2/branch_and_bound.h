#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <climits>
#include <utility>

struct Node {
    std::vector<int> path;
    int cost;
    int level;

    Node(std::vector<int> p, int c, int l) : path(std::move(p)), cost(c), level(l) {}
};

class BranchAndBoundSolver {
public:
    BranchAndBoundSolver(const std::vector<std::vector<int>> &matrix, bool showProgress);

    std::pair<std::vector<int>, int> solveBFS();
    std::pair<std::vector<int>, int> solveDFS();
    std::pair<std::vector<int>, int> solveLowestCost();

private:
    const std::vector<std::vector<int>> &matrix;
    int numCities;
    bool showProgress;
    int calculateCost(const std::vector<int> &path);
    int bound(const Node &node);
};

std::pair<std::vector<int>, int> solveBFS(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit);
std::pair<std::vector<int>, int> solveDFS(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit);
std::pair<std::vector<int>, int> solveLowestCost(const std::vector<std::vector<int>> &matrix, bool showProgress, int timeLimit);

#endif // BRANCH_AND_BOUND_H
