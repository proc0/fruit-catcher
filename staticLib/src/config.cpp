#include <fstream>
#include <cassert>
#include "config.hpp"

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    assert(file && file.is_open());

    int level = 0;
    std::string section;
    std::string line;
    while (std::getline(file, line)) {
        // remove comments
        size_t commentSymbol = line.find(';');
        if (commentSymbol != std::string::npos) {
            line = line.substr(0, commentSymbol);
        }

        // remove leading/trailing whitespace
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);
        if (line.empty()) continue;

        // check for section header
        if (line[0] == '[' && line.back() == ']') {
            section = line.substr(1, line.length() - 2);
            continue;
        }

        std::istringstream stream(line);
        std::string key;
        std::string value;
        if (std::getline(stream, key, '=') && std::getline(stream, value)) {
            if (section == "Debug") {
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
            if (section == "Level") {
                if (key == "id") {
                    level = std::stoi(value);
                    data.levelConfigs[level].id = level;
                } else if (key == "fruitFrequencies") {
                    std::istringstream issVal(value);
                    std::string fruitString;
                    while(std::getline(issVal, fruitString, ',')){
                        try {
                            const int delimiterIndex = fruitString.find(':');
                            const std::string fruitName = fruitString.substr(0, delimiterIndex);
                            const float frequency = std::stof(fruitString.substr(delimiterIndex+1,4));

                            data.levelConfigs[level].fruitFrequencies[fruitName] = frequency;
                        } catch (const std::exception& e) {
                            std::cerr << "Incorrect format for fruitFrequencies.: " << e.what() << std::endl;
                            break;
                        }
                    }
                } else if (key == "dropFrequencyMin") {
                    data.levelConfigs[level].dropFrequencyMin = std::stoi(value);
                } else if (key == "dropFrequencyMax") {
                    data.levelConfigs[level].dropFrequencyMax = std::stoi(value);
                } else if (key == "density") {
                    data.levelConfigs[level].density = std::stoi(value);
                } else if (key == "duration") {
                    data.levelConfigs[level].duration = std::stoi(value);
                } else if (key == "reward") {
                    data.levelConfigs[level].reward = std::stoi(value);
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
