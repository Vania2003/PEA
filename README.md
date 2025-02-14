# Project on Optimization Algorithms for the Traveling Salesman Problem

## Project Description
This repository contains implementations of various algorithms solving the **Traveling Salesman Problem (TSP)**. Each algorithm has been analyzed in terms of time efficiency and accuracy. The analysis is included in the attached reports (which also describe how to run the programs and manage configuration files). In the `script` file, there is a Python script that enables quick retrieval of large amounts of data efficiently.

## Applied Algorithms

### **1. Exact Algorithms**
- **Brute Force (BF)** – a brute-force method checking all possible solutions (complexity O(n!)).
- **Nearest Neighbor (NN)** – a heuristic algorithm selecting the closest unvisited city at each iteration.
- **Random Search** – randomly generating routes and selecting the best attempt.
- **Branch and Bound (B&B):**
  - BFS – breadth-first search
  - DFS – depth-first search
  - Lowest Cost – cost-optimizing strategy

### **2. Heuristic Algorithms**
- **Tabu Search (TS)** – a method using forbidden moves to avoid local minima, utilizing a tabu list and aspiration criterion.
- **Genetic Algorithms (GA)** – an evolution-inspired method based on selection, crossover, and mutation.
