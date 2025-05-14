#include "level.hpp"

Level::Level(const ConfigData &configData){
    for(const SectionLevel &level : configData.levels){
        assert(level.id < LEVEL_COUNT || level.id >= 0);

        LevelSample levelSample;
        int currentFruitRatioIndex = 0;
        MapFruitSample fruitSample;
        for(const auto &pair : level.sample){
            const std::string fruitString = pair.first;
            const float frequency = pair.second;

            try {
                const FruitType fruitType = static_StringToFruit(fruitString);
                fruitSample[fruitType] = frequency;

                if(currentFruitRatioIndex < LENGTH_FRUIT_SAMPLE){
                    int multiplyFruitNum = std::round(frequency * (float)LENGTH_FRUIT_SAMPLE);
                    for(int i=0; i<multiplyFruitNum; i++) {
                        levelSample[currentFruitRatioIndex] = fruitType;
                        currentFruitRatioIndex++;
                        if(currentFruitRatioIndex >= LENGTH_FRUIT_SAMPLE){
                            break;
                        }
                    }
                }

                if(currentFruitRatioIndex >= LENGTH_FRUIT_SAMPLE){
                    break;
                }
            } catch(const std::exception &e){
                std::cerr << "Could not convert string to fruit type, from config file." << e.what() << std::endl;
                break;
            }
        }

        if(currentFruitRatioIndex < LENGTH_FRUIT_SAMPLE){
            int missingFruits = LENGTH_FRUIT_SAMPLE - currentFruitRatioIndex;
            for(int i=0; i<missingFruits; i++){
                levelSample[i+currentFruitRatioIndex] = FruitType::STRAWBERRY;
            }
        }

        levels[level.id] = (LevelData){
            .id = level.id,
            .fruitLevelData = {
                .fruitSample = fruitSample,
                .levelSample = levelSample,
                .levelId = level.id,
                .minDropTime = level.minDropTime,
                .maxDropTime = level.maxDropTime,
                .density = level.density,
            },
            .duration = level.duration,
        };
    }

    if(configData.debug.modeDebug){
        for(const auto &level : levels){
            std::cout << level.id  << std::endl;
            for(auto &freq : level.fruitLevelData.fruitSample){
                std::cout << "fruits: " << static_FruitToString(freq.first) << " " << freq.second << std::endl;
            }
            std::cout << level.fruitLevelData.minDropTime << " " << level.fruitLevelData.maxDropTime << " " << level.duration << " " << std::endl;
        }
    }
}

const LevelData Level::GetCurrentLevel() const {
    return levels[currentLevel];
}

const FruitLevels Level::GetFruitLevelData() const {
    FruitLevels levelData;
    for(int i=0; i<LEVEL_COUNT; i++){
        levelData[i] = levels[i].fruitLevelData;
    }
    return levelData;
}

void Level::NextLevel(){
    if(currentLevel >= LEVEL_COUNT){
        return;
    }
    currentLevel++;
}

void Level::Reset(){
    currentLevel = 0;
}
