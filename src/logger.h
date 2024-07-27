#ifndef MIBAR_LOGGER_H
#define MIBAR_LOGGER_H

#include <string>
#include <iostream>
#include <memory>

enum class LogLvl{
    NONE = 0,
    ERROR,
    WARNING,
    INFO,
    DBUG,
};

class Logger{
public:
    static std::shared_ptr<Logger> GetInstance();

    void Log(const std::string& file, const int line, const std::string& msg, LogLvl level = LogLvl::INFO);

private:
    static std::shared_ptr<Logger> m_logInstance;

    void ConsoleLog(const std::string& msg);
};

#endif