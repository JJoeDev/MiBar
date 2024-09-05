#ifndef MIBAR_LOGGER_H
#define MIBAR_LOGGER_H

#include <iostream>
#include <memory>
#include <string>

#include "utils.h"

#define MI_FILENAME FileName(__FILE__)

enum class LogLevel{
    MESSAGE = 0,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
};

class Logger{
public:
    static std::shared_ptr<Logger> GetInstance();

    void Log(const std::string& file, int line, const std::string& msg, const LogLevel& level);

private:
    LogLevel m_level;

    static std::shared_ptr<Logger> m_logInstance;
};

#endif