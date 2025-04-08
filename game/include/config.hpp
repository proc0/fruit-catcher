#pragma once

#include <string>
#include <map>

typedef struct ConfigDebug {
    bool showCollision;
    bool showFPS;
} ConfigDebug;

typedef struct ConfigData {
    ConfigDebug debug;
} ConfigData;

class Config {
    ConfigData data;

    public:
    Config(const std::string& filename);
    const ConfigData& GetData() const;
};
