    #ifndef CONFIG_H
    #define CONFIG_H

    #include <string>

    struct Config {
        std::string dataFile;
        std::string outputFile;
        int aspirationCriterion;
        bool aspirationPlus;  // Zmieniona nazwa na aspirationPlus
        int margin;           // Początkowy margines dla aspiracji plus
        int maxIterationsWithoutImprovement;
        int repeats;
        bool showProgress;
        int initialSolution;
        int terminationTime;
        bool enableCriticalEventHandling;
        int swapWay;
        int minM; // Minimalna liczba przeglądanych rozwiązań
        int maxM; // Maksymalna liczba przeglądanych rozwiązań
        int plus;
        double tabuListMultiplier;  // Współczynnik dla rozmiaru listy tabu
        double tabuTenureMultiplier;
    };

    #endif // CONFIG_H
