#ifndef MIBAR_PLUGIN_MANAGER_H
#define MIBAR_PLUGIN_MANAGER_H

#include <memory>
#include <vector>
#include <filesystem>
#include <functional>

#include <sol/sol.hpp>

#include "renderer.h"
#include "logger.h"

class PluginManager{
public:
    PluginManager();
    ~PluginManager();

    void ExposeFuncToLua(const std::string& funcName, std::function<void(const std::string&, const int x)> func);
    void RunScripts();

private:
    Logger m_logger;

    std::string m_pluginDir;

    sol::state lua;

    std::vector<std::string> m_plugins;
};

#endif