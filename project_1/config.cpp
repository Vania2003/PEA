#include "config.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>

using namespace std;

void validateConfig(const map<string, string> &configMap) {

    if (configMap.find("data_file") == configMap.end()) {
        throw runtime_error("Brak klucza 'data_file' w pliku konfiguracyjnym.");
    }
    if (configMap.find("output_file") == configMap.end()) {
        throw runtime_error("Brak klucza 'output_file' w pliku konfiguracyjnym.");
    }
    if (configMap.find("repeats") == configMap.end()) {
        throw runtime_error("Brak klucza 'repeats' w pliku konfiguracyjnym.");
    }
    if (configMap.find("show_progress") == configMap.end()) {
        throw runtime_error("Brak klucza 'show_progress' w pliku konfiguracyjnym.");
    }
    if (configMap.find("algorithm") == configMap.end()) {
        throw runtime_error("Brak klucza 'algorithm' w pliku konfiguracyjnym.");
    }
    if (configMap.find("time_limit") == configMap.end()) {
        throw runtime_error("Brak klucza 'time_limit' w pliku konfiguracyjnym.");
    }
}

Config readConfig(const string &configFile) {
    ifstream file(configFile);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku konfiguracyjnego: " << configFile << endl;
        cerr << "Bieżący katalog: " << filesystem::current_path() << endl;
        throw runtime_error("Nie można otworzyć pliku konfiguracyjnego: " + configFile);
    }

    Config config;
    string line;

    while (getline(file, line)) {
        size_t separatorPos = line.find('=');
        if (separatorPos != string::npos) {
            string key = line.substr(0, separatorPos);
            string value = line.substr(separatorPos + 1);

            if (key == "data_file") {
                config.dataFile = value;
            } else if (key == "output_file") {
                config.outputFile = value;
            } else if (key == "repeats") {
                config.repeats = stoi(value);
            } else if (key == "show_progress") {
                config.showProgress = (value == "true");
            } else if (key == "algorithm") {
                config.algorithmType = value;
            } else if (key == "start_vertex") {
                config.startVertex = stoi(value);
            } else if (key == "time_limit") {
                config.timeLimit = stoi(value);
            } else if (key == "run_all_start_vertices") {
                config.runAllStartVertices = (value == "true");
            }
        }
    }

    file.close();
    return config;
}

