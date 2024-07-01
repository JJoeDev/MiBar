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
    int index = 0;
    while(std::getline(in, line)){
        std::istringstream iline(line);
        std::string key;
        if(std::getline(iline, key, '=')){
            std::string value;
            if(std::getline(iline, value)){
                key = Trim(key);
                value = Trim(value);

                m_configs[key] = value;
            }
        }
    }

    for(auto& i : m_configs){
        std::cout << "KEY: " << i.first << " | VALUE: " << i.second << '\n';
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