#ifndef MIBAR_CONFIG_MANAGER
#define MIBAR_CONFIG_MANAGER

#include <memory>
#include <toml++/toml.hpp>

class configManager{
public:
    configManager(const std::string& target = "");
    ~configManager();

private:
    std::unique_ptr<toml::table> m_table = nullptr;
};

#endif