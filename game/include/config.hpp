#pragma once

#include <string>
#include <map>

#include "lib.h"

#define LEVEL_COUNT 3

typedef struct ConfigLevel {
    int id;
    std::map<std::string, float> fruitFrequencies;
    int dropFrequencyMin;
    int dropFrequencyMax;
    int duration;
    int reward;
} ConfigLevel;

typedef struct ConfigDebug {
    bool displayDebug;
    bool showCollisions;
    bool showFPS;
} ConfigDebug;

typedef struct ConfigData {
    ConfigDebug debug;
    ConfigLevel levelConfigs[LEVEL_COUNT];
} ConfigData;

class Config {
    ConfigData data;

    public:
        Config(const std::string& filename);
        const ConfigData& GetData() const;
};
