#include <fstream>

#include "config.hpp"

Config::Config(const std::string& filename) {
    std::ifstream file(filename);
    if (!file || !file.is_open()) {
        std::cerr << "Error opening config file: " << filename << std::endl;
        return;
    }
    
    int currentLevel = 0;
    std::string currentSection;
    std::string line;
    while (std::getline(file, line)) {
        // Remove comments
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        // Remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty()) continue;

        // Check for section header
        if (line[0] == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        std::string value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (currentSection == "Debug") {
                if (key == "showCollisions") {
                    data.debug.showCollisions = value == "true" ? true : false;
                } else if (key == "showFPS") {
                    data.debug.showFPS = value == "true" ? true : false;
                } else if (key == "displayDebug") {
                    data.debug.displayDebug = value == "true" ? true : false;
                } else {
                    std::cerr << "Unknown key in debug section: " << key << std::endl;
                    continue;
                }
            }
            // TODO: add more validation and checks
            if (currentSection == "Level") {
                if (key == "id") {
                    currentLevel = std::stoi(value);
                    data.levelConfigs[currentLevel].id = currentLevel;
                } else if (key == "fruitFrequencies") {
                    std::istringstream issVal(value);
                    std::string fruitString;
                    while(std::getline(issVal, fruitString, ',')){
                        try {
                            const int delimiterIndex = fruitString.find(':');
                            const std::string fruitName = fruitString.substr(0, delimiterIndex);
                            const float frequency = std::stof(fruitString.substr(delimiterIndex+1,4));

                            data.levelConfigs[currentLevel].fruitFrequencies[fruitName] = frequency;
                        } catch (const std::exception& e) {
                            std::cerr << "Incorrect format for fruitFrequencies.: " << e.what() << std::endl;
                            break;
                        }
                    }
                } else if (key == "dropFrequencyMin") {
                    data.levelConfigs[currentLevel].dropFrequencyMin = std::stoi(value);
                } else if (key == "dropFrequencyMax") {
                    data.levelConfigs[currentLevel].dropFrequencyMax = std::stoi(value);
                } else if (key == "density") {
                    data.levelConfigs[currentLevel].density = std::stoi(value);
                } else if (key == "duration") {
                    data.levelConfigs[currentLevel].duration = std::stoi(value);
                } else if (key == "reward") {
                    data.levelConfigs[currentLevel].reward = std::stoi(value);
                } else {
                    std::cerr << "Unknown key in level section: " << key << std::endl;
                    continue;
                }
            }
        }
    }

    file.close();
}

const ConfigData& Config::GetData() const {
    return data;
}
