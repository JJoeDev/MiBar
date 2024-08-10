#include <algorithm>
#include <filesystem>
#include <fstream>

#include "configParser.h"

ConfigParser::ConfigParser(){

}

void ConfigParser::Parse(const std::string& file){
    auto configDir = std::filesystem::path(getenv("HOME")) / ".config/MiBar";
    
    if(file.empty()){
        configDir /= "config";
    }
    else{
        configDir /= file;
    }

    configDir += ".bar";

    // Parsing config file
    std::fstream infile(configDir);

    std::string line;
    while(std::getline(infile, line)){
        std::istringstream isLine(line);
        std::string key;

        if(std::getline(isLine, key, ':')){
            if(key.find('*') != std::string::npos) continue;

            std::string value;
            if(std::getline(isLine, value)){
                value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
                m_configs[key] = value;
            }
        }
    }
}

const std::string ConfigParser::GetConfig(const int key) const {
    auto it = m_configs.find(m_configTable[key]);
    return it->second;
}