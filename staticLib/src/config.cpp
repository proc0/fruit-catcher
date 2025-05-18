#include <fstream>
#include <sstream>

#include "config.hpp"

Config::Config(const std::string& filepath) {
    std::ifstream file(filepath);
    assert(file && file.is_open());

    SectionLevel level {};
    
    std::string currentSection = "";
    std::string previousSection = "";
    
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
            previousSection = currentSection;
            currentSection = line.substr(1, line.length() - 2);
            
            if(previousSection == "Level"){
                // store processed level section
                data.levels.push_back(level);
            }

            if(currentSection == "Level"){
                // clear level cache
                level = {};
            }
            continue;
        }

        
        std::string key;
        std::string value;
        std::istringstream streamLine(line);

        if (std::getline(streamLine, key, '=') && std::getline(streamLine, value)) {
            if (currentSection == "Debug") {
                if (key == "showCages") {
                    data.debug.showCages = value == "true" ? true : false;
                } else if (key == "showFPS") {
                    data.debug.showFPS = value == "true" ? true : false;
                } else if (key == "modeDebug") {
                    data.debug.modeDebug = value == "true" ? true : false;
                } else {
                    std::cerr << "Unknown key in [Debug]: " << key << std::endl;
                    continue;
                }
            }

            if (currentSection == "Level") {

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
                            std::cerr << "Error parsing [Level] sample: " << e.what() << std::endl;
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
                    std::cerr << "Unknown key in [Level]: " << key << std::endl;
                    continue;
                }
            }
        }
    }

    if(previousSection == "Level"){
        // store last level section
        data.levels.push_back(level);
    }

    file.close();
}

const ConfigData& Config::GetData() const {
    return data;
}
