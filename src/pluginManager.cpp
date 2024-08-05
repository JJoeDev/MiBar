#include "pluginManager.h"

PluginManager::PluginManager(){
    m_pluginDir = std::filesystem::path(getenv("HOME")) / ".config/MiBar/plugins";

    if(std::filesystem::create_directories(m_pluginDir)){
        m_logger.Log("", 0, "Created plugin directory at: " + m_pluginDir);
    }
    else{
        m_logger.Log("", 0, "Plugin directory found at: " + m_pluginDir);
    }

    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::string, sol::lib::io);

    for(auto&& entry : std::filesystem::directory_iterator(m_pluginDir)){
        if(entry.path().extension() == ".lua"){
            m_plugins.emplace_back(entry.path());
            m_logger.Log(__FILE_NAME__, 0, "Found plugin: " + entry.path().filename().string());
        }
    }
}

PluginManager::~PluginManager(){
    
}

void PluginManager::ExposeFuncToLua(const std::string& funcName, std::function<void(const std::string&, const int x)> func){
    lua[funcName] = [func](const std::string& arg, const int x){
        func(arg, x);
    };
}

void PluginManager::ExposeFuncToLua(const std::string& funcName, std::function<void(int x, int y, int w, int h, int idx)> func){
    lua[funcName] = [func](int x, int y, int w, int h, int idx){
        func(x, y, w, h, idx);
    };
}

void PluginManager::RunScripts(){
    for(auto&& i : m_plugins){
        lua.script_file(i);
    }
}