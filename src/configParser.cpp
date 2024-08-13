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

    std::ifstream infile(configDir);
    if(!infile){
        std::cerr << "Error opening config file! : " << configDir << '\n';
        return;
    }

    std::stringstream lineStream;
    std::string line, key, value;

    while(std::getline(infile, line)){
        if(line.empty() || line[0] == '*') continue;

        lineStream.clear();
        lineStream << line;

        if(!std::getline(lineStream, key, ':')){
            std::cerr << "Invalid config! : " << line << '\n';
            continue;
        }

        if(std::getline(lineStream, value)){
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
            m_configs[key] = value;
        }
        else{
            std::cerr << "Missing value for key: " << key << '\n';
        }
    }
}

const std::string ConfigParser::GetConfig(const int key) const {
    if(auto it = m_configs.find(m_configTable[key]); it != m_configs.end())
        return it->second;

    return "0x0";
}