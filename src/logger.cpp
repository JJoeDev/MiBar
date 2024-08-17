#include "logger.h"

std::shared_ptr<Logger> Logger::m_logInstance;

std::shared_ptr<Logger> Logger::GetInstance(){
    if(m_logInstance == nullptr){
        m_logInstance = std::shared_ptr<Logger>(new Logger());
    }

    return m_logInstance;
}

// Terminal color documentation >>> https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
void Logger::Log(const std::string& file, int line, const std::string& msg, const LogLevel& level){
#ifdef NDEBUG
    if(level == LogLevel::DEBUG) return;
#endif

    std::string logPrefix("");

    switch(level){
    case LogLevel::MESSAGE: logPrefix = "\033[1;36m[ MESSAGE ]\033[0m "; break;
    case LogLevel::ERROR:   logPrefix = "\033[1;31m[  ERROR  ]\033[0m "; break;
    case LogLevel::WARNING: logPrefix = "\033[1;33m[ WARNING ]\033[0m "; break;
    case LogLevel::INFO:    logPrefix = "\033[1;32m[  INFO   ]\033[0m "; break;
    case LogLevel::DEBUG:   logPrefix = "\033[7;33m[  DEBUG  ]\033[0m "; break;
    default: break;
    }

    std::string message(logPrefix + msg + " -> " + file + " : " + std::to_string(line));
    std::cout << message << '\n';
}