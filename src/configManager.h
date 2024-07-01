#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

// https://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c

#include <string>
#include <unordered_map>

class cfgManager{
public:
    cfgManager();
    ~cfgManager();

    bool Parse();

    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& GetConfigFile() const{ return m_configs; }

private:
    std::string Trim(const std::string& str);

    std::string m_cfgPath;
    const std::string m_cfgFile = "/config.mi";

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_configs;
};

#endif