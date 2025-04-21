#include "level.hpp"

Level::Level(const ConfigData &configData){
    for(const ConfigLevel &levelConfig : configData.levelConfigs){
        //TODO: look into making this an assert
        if(levelConfig.id >= GAME_LEVELS_NUMBER || levelConfig.id < 0){
            std::cerr << "Too many levels defined in config file, or wrong level indices." << std::endl;
            break;
        }

        std::map<FruitType, float> _frequencies;
        for(const auto &pair : levelConfig.fruitFrequencies){
            const std::string fruitString = pair.first;
            const float frequency = pair.second;

            try {
                const FruitType fruitType = static_StringToFruit(fruitString);
                _frequencies[fruitType] = frequency;
            } catch(const std::exception &e){
                std::cerr << "Could not convert string to fruit type, from config file." << e.what() << std::endl;
                break;
            }
        }

        levels[levelConfig.id] = {
            id: levelConfig.id,
            fruitLevelData: {
                levelId: levelConfig.id,
                fruitFrequencies: _frequencies,
                dropFrequencyMin: levelConfig.dropFrequencyMin,
                dropFrequencyMax: levelConfig.dropFrequencyMax,
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
    try {

        return levels[0];
    } catch (const std::exception &e) { 
        std::cout << "blah blah" << '\n';
        return levels[0];
    }
}
