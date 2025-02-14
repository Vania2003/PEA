#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "config.h"
#include <vector>

class TabuSearch {
private:
    std::vector<std::vector<int>> distanceMatrix; // Macierz odległości
    Config config; // Parametry konfiguracyjne
    int numCities; // Liczba miast
    int bestCost; // Najlepszy koszt znalezionego rozwiązania
    std::vector<std::vector<int>> tabuList; // Lista tabu
    int tabuTenure; // Kadencja tabu
    int tabuListSize; // Rozmiar listy tabu (maksymalna liczba aktywnych zakazów)
    int iterations; // Liczba iteracji
    int maxIterations; // Maksymalna liczba iteracji
    int aspirationCriterion; // Kryterium aspiracji
    int iterationsWithoutImprovement; // Liczba iteracji bez poprawy
    int maxIterationsWithoutImprovement; // Maksymalna liczba iteracji bez poprawy

    // Metody pomocnicze
    std::vector<int> generateInitialSolution(); // Generowanie początkowego rozwiązania
    int calculateCost(const std::vector<int>& solution); // Obliczanie kosztu rozwiązania
    void swapCities(std::vector<int>& solution, int i, int j); // Zamiana miast w rozwiązaniu
    bool isTabu(int city1, int city2) const; // Sprawdzanie czy ruch jest tabu
    bool aspirationCriteria(int currentCost) const; // Sprawdzanie kryterium aspiracji
    bool isAllowed(int city1, int city2, int currentCost) const;
    void updateTabuList(int city1, int city2); // Aktualizacja listy tabu
    void decrementTabuList(); // Dekrementacja listy tabu, aby ograniczyć aktywne zakazy

    // Metody przeglądu sąsiedztwa
    std::vector<int> applyTwoOpt(const std::vector<int>& solution, int& bestNeighborCost); // Przegląd 2-opt
    std::vector<int> applyCitySwap(const std::vector<int>& solution, int& bestNeighborCost); // Przegląd zamiany miast

public:
    TabuSearch(const std::vector<std::vector<int>>& matrix, const Config& config); // Konstruktor
    std::vector<int> solve(); // Główna metoda rozwiązująca problem TSP
};

#endif // TABU_SEARCH_H
