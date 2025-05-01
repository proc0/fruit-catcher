#include "level.hpp"

Level::Level(const ConfigData &configData){
    for(const ConfigLevel &levelConfig : configData.levelConfigs){
        //TODO: look into making this an assert
        if(levelConfig.id >= LEVEL_COUNT || levelConfig.id < 0){
            std::cerr << "Too many levels defined in config file, or wrong level indices." << std::endl;
            break;
        }

        FruitSample fruitSample;
        int currentFruitRatioIndex = 0;
        std::map<FruitType, float> _frequencies;
        for(const auto &pair : levelConfig.fruitFrequencies){
            const std::string fruitString = pair.first;
            const float frequency = pair.second;

            try {
                const FruitType fruitType = static_StringToFruit(fruitString);
                _frequencies[fruitType] = frequency;

                if(currentFruitRatioIndex < FRUIT_TYPE_COUNT){
                    int multiplyFruitNum = frequency * 10.0f;
                    for(int i=0; i<multiplyFruitNum; i++) {
                        fruitSample[currentFruitRatioIndex] = fruitType;
                        currentFruitRatioIndex++;
                        if(currentFruitRatioIndex >= FRUIT_TYPE_COUNT){
                            break;
                        }
                    }
                }
            } catch(const std::exception &e){
                std::cerr << "Could not convert string to fruit type, from config file." << e.what() << std::endl;
                break;
            }
        }

        levels[levelConfig.id] = (LevelData){
            id: levelConfig.id,
            fruitLevelData: {
                levelId: levelConfig.id,
                fruitFrequencies: _frequencies,
                fruitSample: fruitSample,
                dropFrequencyMin: levelConfig.dropFrequencyMin,
                dropFrequencyMax: levelConfig.dropFrequencyMax,
                density: levelConfig.density,
            },
            duration: levelConfig.duration,
            reward: levelConfig.reward,
        };
    }

    // debug
    for(const auto &level : levels){
        std::cout << level.id  << std::endl;
        for(auto &freq : level.fruitLevelData.fruitFrequencies){
            std::cout << "fruits: " << static_FruitToString(freq.first) << " " << freq.second << std::endl;
        }
        std::cout << level.fruitLevelData.dropFrequencyMin << " " << level.fruitLevelData.dropFrequencyMax << " " << level.duration << " " << level.reward << std::endl;
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
