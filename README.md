# Projekt Algorytmów Optymalizacyjnych dla Problemu Komiwojażera

## Opis Projektu
To repozytorium zawiera implementacje różnych algorytmów rozwiązujących **Problem Komiwojażera (TSP - Traveling Salesman Problem)**. Algorytmy te zostały zaimplementowane w ramach laboratoriów z **Projektowania Efektywnych Algorytmów**. Każdy algorytm został przeanalizowany pod kątem wydajności czasowej i dokładności wyników.

## 🔍 Zastosowane Algorytmy

### **1️⃣ Algorytmy dokładne**
- **Brute Force (BF)** – metoda siłowa sprawdzająca wszystkie możliwe rozwiązania (złożoność O(n!)).
- **Nearest Neighbor (NN)** – heurystyczny algorytm wybierający w każdej iteracji najbliższe nieodwiedzone miasto.
- **Random Search** – losowe generowanie tras i wybór najlepszej z prób.
- **Branch and Bound (B&B)**:
  - BFS – przeszukiwanie szerokościowe
  - DFS – przeszukiwanie głębokościowe
  - Lowest Cost – strategia optymalizująca koszt przejścia

### **2️⃣ Algorytmy heurystyczne**
- **Tabu Search (TS)** – metoda z zakazami unikania lokalnych minimów, wykorzystująca listę tabu i kryterium aspiracji.
- **Algorytmy genetyczne (GA)** – metoda inspirowana ewolucją, bazująca na selekcji, krzyżowaniu i mutacji.

## ⚙️ Struktura katalogów
```
/PEA/
│── /bf_nn_random/      # Implementacja algorytmów BF, NN, Random
│── /branch_and_bound/  # Implementacja metody podziału i ograniczeń
│── /tabu_search/       # Implementacja algorytmu Tabu Search
│── /genetic_algorithm/ # Implementacja algorytmów genetycznych
│── /data/              # Pliki z danymi testowymi (instancje TSP)
│── /results/           # Wyniki eksperymentów
│── README.md           # Dokumentacja projektu
```

## 📊 Dane testowe
Dane wejściowe pochodzą ze strony **TSPLIB** oraz **Dr. Jarosława Mierzwy** i zawierają instancje o różnych rozmiarach (4-101 miast). Pliki konfiguracyjne umożliwiają zmianę parametrów testów.

## 🏗️ Instalacja i Uruchomienie
### **1️⃣ Klonowanie repozytorium**
```bash
git clone https://github.com/TWOJNICK/PEA.git
cd PEA
```

### **2️⃣ Kompilacja i uruchomienie (przykład dla BFS)**
```bash
g++ branch_and_bound/bfs.cpp -o bfs
./bfs data/tsp_10.txt
```

## 🎮 Konfiguracja algorytmów
Pliki `.config` zawierają opcje sterujące działaniem algorytmów, m.in.:
- **data_file**: ścieżka do pliku z instancją TSP
- **algorithm**: wybrany algorytm (BF, NN, B&B, TS, GA)
- **time_limit**: maksymalny czas działania algorytmu
- **mutation_probability**: prawdopodobieństwo mutacji w GA
- **tabu_list_size**: rozmiar listy tabu w Tabu Search

## 📌 Wyniki i Analiza
Wyniki eksperymentów zapisywane są w plikach `.xls` i obejmują:
- Czas wykonania algorytmów
- Błędy względem rozwiązania optymalnego
- Efektywność algorytmów dla różnych typów instancji (symetryczne/asymetryczne)

## 👨‍💻 Autor
- **Ivan Hancharyk** - [GitHub](https://github.com/TWOJNICK)

## 📜 Licencja
Projekt udostępniony na licencji **MIT** - można go swobodnie modyfikować i używać do własnych celów.
