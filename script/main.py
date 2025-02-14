import os
import time
import pandas as pd
from subprocess import Popen, PIPE

# Ścieżki do plików instancji TSP
instances = [
    "ftv33.txt",
    "tsp_10.txt",
    "tsp8_as.txt"
]

# Nazwa pliku wyjściowego
output_excel_file = "tabu_search_results.xlsx"
output_text_file = "tabu_search_results.txt"

# Lista wyników
results = []

# Testowanie każdej instancji
for instance in instances:
    print(f"Testowanie instancji: {instance}")

    # Modyfikacja pliku config.txt, aby wskazywał na bieżącą instancję
    with open("config.txt", "r") as file:
        config_lines = file.readlines()

    for i, line in enumerate(config_lines):
        if line.startswith("data_file"):
            config_lines[i] = f"data_file={instance}\n"

    with open("config.txt", "w") as file:
        file.writelines(config_lines)

    # Uruchomienie programu main.exe i zbieranie danych
    start_time = time.time()
    process = Popen(["./main.exe"], stdout=PIPE, stderr=PIPE, text=True)
    stdout, stderr = process.communicate()
    execution_time = time.time() - start_time

    if process.returncode != 0:
        print(f"Błąd podczas testowania instancji {instance}: {stderr}")
        continue

    # Parsowanie wyników z konsoli
    min_cost = None
    abs_error = None
    rel_error = None

    for line in stdout.splitlines():
        if "Minimalny koszt" in line:
            min_cost = int(line.split(":")[-1].strip())
        elif "Blad bezwzgledny" in line:
            abs_error = int(line.split(":")[-1].strip())
        elif "Blad wzgledny" in line:
            rel_error = float(line.split(":")[-1].replace("%", "").strip())

    # Dodanie wyników do listy
    results.append({
        "Instancja": instance,
        "Średni koszt": min_cost,
        "Błąd bezwzględny": abs_error,
        "Błąd względny (%)": rel_error,
        "Średni czas (s)": execution_time
    })

# Konwersja wyników do DataFrame
df_results = pd.DataFrame(results)

# Zapis wyników do pliku Excel
df_results.to_excel(output_excel_file, index=False)
print(f"Wyniki zapisano do pliku {output_excel_file}")

# Zapis wyników do pliku tekstowego
df_results.to_csv(output_text_file, index=False, sep="\t")
print(f"Wyniki zapisano do pliku {output_text_file}")
