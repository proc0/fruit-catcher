#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

typedef std::unordered_map<std::string, float> MapSample;

typedef struct SectionLevel {
    MapSample sample;
    int id;
    int minDropTime;
    int maxDropTime;
    int duration;
    int density;
} SectionLevel;

typedef struct SectionDebug {
    bool modeDebug;
    bool showCages;
    bool showFPS;
} SectionDebug;

typedef struct ConfigData {
    SectionDebug debug;
    std::vector<SectionLevel> levels;
} ConfigData;

class Config {
    ConfigData data;

    public:
        Config(const std::string& filepath);
        ~Config()=default;
        const ConfigData& GetData() const;
};
