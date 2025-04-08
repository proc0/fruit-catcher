#include "config.hpp"

#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

Config::Config(const std::string& filename) {
    std::ifstream file(filename);
    if (!file || !file.is_open()) {
        std::cerr << "Error opening config file: " << filename << std::endl;
        return;
    }
    
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
                if (key == "showCollision") {
                    data.debug.showCollision = value == "true" ? true : false;
                } else if (key == "showFPS") {
                    data.debug.showFPS = value == "true" ? true : false;
                }
            }
        }
    }

    file.close();
}

const ConfigData& Config::GetData() const {
    return data;
}
