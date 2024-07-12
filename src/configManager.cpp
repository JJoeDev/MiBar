#include <iostream>
#include <filesystem>

#define TOML_EXCEPTIONS 0
#include "configManager.h"

namespace fs = std::filesystem;

configManager::configManager(const std::string& target){
    std::string file = fs::path(getenv("HOME")) / ".config/MiBar/";
    
    if(target.empty()){
        file.append("config.toml");
    }
    else{
        file.append(target + ".toml");
    }

    std::cout << file << '\n';

    toml::parse_result result = toml::parse_file(file);
    if(!result){
        std::cerr << "[ERROR] Config parsing failed: " << result.error() << '\n';
        return;
    }

    m_table = std::make_unique<toml::table>(std::move(result).table());
}

configManager::~configManager(){

}