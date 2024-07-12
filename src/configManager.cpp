#include <iostream>
#include <filesystem>

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

    try{
        toml::table tbl = toml::parse_file(file);

        std::cout << "bar: " << tbl["bar"] << '\n';
    }
    catch(const toml::parse_error& err){
        std::cerr << "\nTOML ERROR IN: " << err.source().path << "\nDESC: " << err.description() << "\n" << err.source().begin;
    }

}

configManager::~configManager(){

}