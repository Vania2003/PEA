#include "genetic_algorithm.h"
#include <algorithm>
#include <chrono>
#include <numeric>
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm(const std::vector<std::vector<int>> &matrix, const Config &cfg)
    : distanceMatrix(matrix), config(cfg), randomEngine(std::random_device{}()) {}

void GeneticAlgorithm::initializePopulation() {
    population.resize(config.populationSize, std::vector<int>(distanceMatrix.size()));
    for (auto &individual : population) {
        std::iota(individual.begin(), individual.end(), 0);
        std::shuffle(individual.begin(), individual.end(), randomEngine);
    }
}

void GeneticAlgorithm::calculateFitness() {
    fitness.clear();
    for (const auto &individual : population) {
        fitness.push_back(calculatePathCost(individual));
    }
}

std::pair<int, int> GeneticAlgorithm::rouletteWheelSelection() {
    int totalFitness = std::accumulate(fitness.begin(), fitness.end(), 0);
    std::uniform_int_distribution<int> dist(0, totalFitness);
    int randValue1 = dist(randomEngine);
    int randValue2 = dist(randomEngine);

    auto select = [&](int randValue) {
        int sum = 0;
        for (int i = 0; i < fitness.size(); ++i) {
            sum += fitness[i];
            if (sum >= randValue) return i;
        }
        return static_cast<int>(fitness.size() - 1);
    };

    int first = select(randValue1);
    int second = select(randValue2);
    return {first, second};
}

std::pair<int, int> GeneticAlgorithm::tournamentSelection() {
    std::uniform_int_distribution<int> dist(0, config.populationSize - 1);
    int first = dist(randomEngine);
    int second = dist(randomEngine);
    return fitness[first] < fitness[second] ? std::make_pair(first, second) : std::make_pair(second, first);
}

std::vector<int> GeneticAlgorithm::orderCrossover(const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::vector<int> child(parent1.size(), -1);
    std::uniform_int_distribution<int> dist(0, parent1.size() - 1);
    int start = dist(randomEngine);
    int end = dist(randomEngine);
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    int currentIdx = (end + 1) % parent2.size();
    for (int i = 0; i < parent2.size(); ++i) {
        int idx = (end + 1 + i) % parent2.size();
        if (std::find(child.begin(), child.end(), parent2[idx]) == child.end()) {
            child[currentIdx] = parent2[idx];
            currentIdx = (currentIdx + 1) % parent2.size();
        }
    }

    return child;
}

std::vector<int> GeneticAlgorithm::pmxCrossover(const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::vector<int> child(parent1.size(), -1);
    std::uniform_int_distribution<int> dist(0, parent1.size() - 1);
    int start = dist(randomEngine);
    int end = dist(randomEngine);
    if (start > end) std::swap(start, end);

    for (int i = start; i <= end; ++i) {
        child[i] = parent1[i];
    }

    auto mapValue = [&](int value) {
        while (std::find(child.begin() + start, child.begin() + end + 1, value) != child.end()) {
            value = parent2[std::distance(parent1.begin(), std::find(parent1.begin(), parent1.end(), value))];
        }
        return value;
    };

    for (int i = 0; i < parent2.size(); ++i) {
        if (i < start || i > end) {
            child[i] = mapValue(parent2[i]);
        }
    }

    return child;
}

void GeneticAlgorithm::reciprocalExchangeMutation(std::vector<int> &individual) {
    std::uniform_int_distribution<int> dist(0, individual.size() - 1);
    int i = dist(randomEngine);
    int j = dist(randomEngine);
    if (i != j) std::swap(individual[i], individual[j]);
}

void GeneticAlgorithm::partialReplacement() {
    std::vector<std::vector<int>> newPopulation;

    // 1. Sortuj populację według fitness (rosnąco, lepsze rozwiązania na początku)
    std::vector<int> indices(population.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](int i, int j) {
        return fitness[i] < fitness[j];
    });

    // 2. Dodaj najlepsze x% (elita) do nowej populacji
    int eliteCount = static_cast<int>(config.populationSize * config.elitePercentage); // 10% populacji
    for (int i = 0; i < eliteCount; ++i) {
        newPopulation.push_back(population[indices[i]]);
    }

    // 3. Wypełnij pozostałe miejsca w populacji nowymi rozwiązaniami
    while (newPopulation.size() < config.populationSize) {
        std::pair<int, int> parents;

        // Wybór rodziców (metoda selekcji: turniejowa lub ruletka)
        if (config.selectionMethod == "roulette") {
            parents = rouletteWheelSelection();
        } else {
            parents = tournamentSelection();
        }

        // Krzyżowanie
        std::vector<int> child;
        if (std::uniform_real_distribution<double>(0, 1)(randomEngine) < config.crossoverProbability) {
            if (config.crossoverMethod == "pmx") {
                child = pmxCrossover(population[parents.first], population[parents.second]);
            } else {
                child = orderCrossover(population[parents.first], population[parents.second]);
            }
        } else {
            child = population[parents.first]; // Brak krzyżowania, dziecko = rodzic 1
        }

        // Mutacja
        if (std::uniform_real_distribution<double>(0, 1)(randomEngine) < config.mutationProbability) {
            reciprocalExchangeMutation(child);
        }

        newPopulation.push_back(child);
    }

    // 4. Zastąp starą populację nową populacją
    population = std::move(newPopulation);
}

int GeneticAlgorithm::calculatePathCost(const std::vector<int> &path) {
    int cost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        cost += distanceMatrix[path[i]][path[i + 1]];
    }
    cost += distanceMatrix[path.back()][path[0]];
    return cost;
}

std::vector<int> GeneticAlgorithm::solve() {
    initializePopulation();
    int bestFitness = std::numeric_limits<int>::max();
    int stagnationCounter = 0;

    // Start timer for timeout
    auto startTime = std::chrono::steady_clock::now(); // Start the timer

    for (int generation = 0; generation < config.maxGenerations; ++generation) {
        calculateFitness();

        // Check for improvement
        auto minIt = std::min_element(fitness.begin(), fitness.end());
        if (*minIt < bestFitness) {
            bestFitness = *minIt;
            stagnationCounter = 0; // Reset stagnation counter
        } else {
            stagnationCounter++;
        }

        // Check timeout in milliseconds
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
        if (elapsedTime >= config.timeout * 1000) { // Convert timeout from seconds to milliseconds
            std::cout << "Algorytm zatrzymany z powodu przekroczenia limitu czasu (" << elapsedTime / 1000.0 << " sekund)." << std::endl;
            break;
        }

        // Replace population
        partialReplacement();

        // Optional: Show progress
        if (config.showProgress) {
            std::cout << "Generacja " << generation << ", Najlepszy koszt: " << bestFitness << std::endl;
        }
    }

    // Return the best solution
    auto bestIt = std::min_element(fitness.begin(), fitness.end());
    return population[std::distance(fitness.begin(), bestIt)];
}
