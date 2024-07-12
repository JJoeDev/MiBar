#ifndef MIBAR_CONFIG_MANAGER
#define MIBAR_CONFIG_MANAGER

#include <unordered_map>
#include <toml++/toml.hpp>

class configManager{
public:
    configManager(const std::string& target = "");
    ~configManager();

private:

};

#endif