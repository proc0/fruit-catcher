#pragma once

#include <map>

#include "lib.h"
#include "fruits.hpp"
#include "config.hpp"

typedef struct LevelData {
    int id;
    FruitLevelData fruitLevelData;
    int duration;
    int reward;
} LevelData;

class Level {
    LevelData levels[GAME_LEVELS_NUMBER];
    int currentLevel = 0;

    public:
        Level(const ConfigData&);
        ~Level() = default;
        const LevelData GetCurrentLevel() const;
        // void UpdateLevel(int currentLevel);
};
