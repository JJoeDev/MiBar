#include "configManager.h"

#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = ::std::filesystem;

cfgManager::cfgManager(){
    m_cfgPath = fs::path(getenv("HOME")) / ".config/MiBar";

    if(fs::create_directory(m_cfgPath)){
        std::cout << "Creating MiBar config directory at: " << m_cfgPath << '\n';
    }
    else{
        std::cout << "Config directory is ready at: " << m_cfgPath << '\n';
    }
}

cfgManager::~cfgManager(){

}

bool cfgManager::Parse(){
    std::ifstream in(m_cfgPath + m_cfgFile);

    std::string line;
    while(std::getline(in, line)){
        std::istringstream iline(line);
        std::string fullKey;

        if(std::getline(iline, fullKey, '=')){
            std::string value;

            if(std::getline(iline, value)){
                fullKey = Trim(fullKey);
                value = Trim(value);

                size_t colonPos = fullKey.find(':');
                if(colonPos != std::string::npos){
                    std::string module = fullKey.substr(0, colonPos);
                    std::string key = fullKey.substr(colonPos + 1);

                    m_configs[module][key] = value;
                }
                else {
                    m_configs["default"][fullKey] = value;
                }
            }
        }
    }

    return true;
}

// Privates

std::string cfgManager::Trim(const std::string& str){
    size_t first = str.find_first_not_of(' ');
    if(std::string::npos == first){
        return str;
    }

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}