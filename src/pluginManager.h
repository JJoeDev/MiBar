#ifndef MIBAR_PLUGIN_MANAGER_H
#define MIBAR_PLUGIN_MANAGER_H

#include <memory>
#include <vector>
#include <filesystem>
#include <functional>

#include "solInclude.h"

#include "renderer.h"
#include "logger.h"

class PluginManager{
public:
    PluginManager();
    ~PluginManager();

    void ExposeFuncToLua(const std::string& funcName, std::function<void(const std::string&, ALIGNMENT, const int)> func);
    void ExposeFuncToLua(const std::string& funcName, std::function<void(int x, int y, int w, int h, int idx)> func);

    void RunScripts();

private:
    std::shared_ptr<Logger> m_logger;

    std::string m_pluginDir = "";

    void GenericExposes();

    sol::state lua;

    std::vector<std::pair<char, std::string>> m_plugins;
};

#endif