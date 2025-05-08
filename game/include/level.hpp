#pragma once

#include "fruits.hpp"

typedef struct LevelData {
    int id;
    FruitLevel fruitLevelData;
    int duration;
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
