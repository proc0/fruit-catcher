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
    LevelData levels[LEVEL_COUNT];
    int currentLevel = 0;

    public:
        Level(const ConfigData&);
        ~Level() = default;
        void NextLevel();
        void Reset();
        const LevelData GetCurrentLevel() const;
        const FruitLevels GetFruitLevelData() const;
};
