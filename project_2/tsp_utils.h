#ifndef TSP_UTILS_H
#define TSP_UTILS_H

#include <vector>
#include <string>

class TSPUtils {
public:
    struct Config {
        std::string dataFile;        // Ścieżka do pliku z danymi
        std::string outputFile;      // Ścieżka do pliku wyjściowego (Excel)
        std::string algorithmType;   // Typ algorytmu (BFS, DFS, LowestCost itp.)
        int repeats;                 // Liczba powtórzeń testu
        int timeLimit;               // Limit czasu działania algorytmu
        bool showProgress;           // Czy pokazywać postęp algorytmu
    };

    static Config readConfig(const std::string &filename);

    static void validateConfig(const Config &config);

    static void readMatrix(const std::string &filename, int &numCities, std::vector<std::vector<int>> &matrix);

    static void saveResults(const std::string &outputFile, const std::string &dataFile, const std::vector<int> &bestPath, int minCost, const std::vector<double> &executionTimes);

    static void displayResults(const std::string &dataFile, int minCost, const std::vector<int> &bestPath, double execTime);

private:
    static bool isValidAlgorithm(const std::string &algorithmType);
};

#endif // TSP_UTILS_H
