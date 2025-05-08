#include <fstream>
#include <sstream>

#include "config.hpp"

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    assert(file && file.is_open());

    std::string line;
    std::string section;
    SectionLevel level = {};

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

        std::string key;
        std::string value;
        std::istringstream streamLine(line);

        if (std::getline(streamLine, key, '=') && std::getline(streamLine, value)) {
            if (section == "Debug") {
                if (key == "showCages") {
                    data.debug.showCages = value == "true" ? true : false;
                } else if (key == "showFPS") {
                    data.debug.showFPS = value == "true" ? true : false;
                } else if (key == "modeDebug") {
                    data.debug.modeDebug = value == "true" ? true : false;
                } else {
                    std::cerr << "Unknown key in debug section: " << key << std::endl;
                    continue;
                }
            }

            if (section == "Level") {
                if (key == "id") {
                    level.id = std::stoi(value);
                } else if (key == "sample") {
                    std::istringstream streamValue(value);
                    std::string fruitString;
                    while(std::getline(streamValue, fruitString, ',')){
                        try {
                            const int splitSymbol = fruitString.find(':');
                            const std::string fruitName = fruitString.substr(0, splitSymbol);
                            const float frequency = std::stof(fruitString.substr(splitSymbol+1,4));

                            level.sample[fruitName] = frequency;
                        } catch (const std::exception& e) {
                            std::cerr << "Incorrect format for sample.: " << e.what() << std::endl;
                            break;
                        }
                    }
                } else if (key == "minDropTime") {
                    level.minDropTime = std::stoi(value);
                } else if (key == "maxDropTime") {
                    level.maxDropTime = std::stoi(value);
                } else if (key == "density") {
                    level.density = std::stoi(value);
                } else if (key == "duration") {
                    level.duration = std::stoi(value);
                } else {
                    std::cerr << "Unknown key in level section: " << key << std::endl;
                    continue;
                }

                data.levels.push_back(level);
            }
        }
    }

    file.close();
}

const ConfigData& Config::GetData() const {
    return data;
}
