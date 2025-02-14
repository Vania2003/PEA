#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

struct Config {
    std::string dataFile;
    std::string outputFile;
    int repeats;
    bool showProgress;
    std::string algorithmType;
    int startVertex;
    int timeLimit;
    bool runAllStartVertices;
};

Config readConfig(const std::string &configFile);

#endif // CONFIG_H
