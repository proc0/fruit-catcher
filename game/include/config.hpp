#pragma once

#include <string>
#include <map>

#include "lib.h"

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
    ConfigLevel levelConfigs[GAME_LEVELS_NUMBER];
} ConfigData;

class Config {
    ConfigData data;

    public:
        Config(const std::string& filename);
        const ConfigData& GetData() const;
};
