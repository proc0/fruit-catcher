#pragma once

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
        const std::array<FruitLevelData, GAME_LEVELS_NUMBER> GetFruitLevelData() const;
        // void UpdateLevel(int currentLevel);
};
