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

    /**
        A function to print a detailed string to the console with std::cout

        @param file Use "" to not include a file or use a macro like __FILE_NAME__ or __FILE__
        @param line Use 0 to not include a line number or use __LINE__
        @param msg This is a custom string to print to the console
        @param level This is the severity of the log. Default level is LogLvl::INFO
    */
    void Log(const std::string& file, const int line, const std::string& msg, LogLvl level = LogLvl::INFO);

private:
    static std::shared_ptr<Logger> m_logInstance;

    void ConsoleLog(const std::string& msg);
};

#endif