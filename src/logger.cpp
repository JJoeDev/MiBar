#include "logger.h"

std::shared_ptr<Logger> Logger::m_logInstance;

std::shared_ptr<Logger> Logger::GetInstance(){
    if(!m_logInstance){
        m_logInstance = std::shared_ptr<Logger>(new Logger());
    }

    return m_logInstance;
}

void Logger::Log(const std::string& file, const int line, const std::string& msg, LogLvl level){
    std::string type;
    
    // Terminal color documentation >>> https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal

    switch(level){
    case LogLvl::NONE:
        type = "";
        break;
    case LogLvl::ERROR:
        type = "\033[31m[ERROR] \033[0m";
        break;
    case LogLvl::WARNING:
        type = "\033[33m[WARNING] \033[0m";
        break;
    case LogLvl::INFO:
        type = "\033[1m\033[32m[INFO] \033[0m";
        break;
    case LogLvl::DBUG:
        type = "\033[7m\033[33m[DEBUG] \033[0m";
        break;
    }

#ifndef NDEBUG 
    ConsoleLog(std::string{type + msg + " : " + file + (line > 0 ? " : " + std::to_string(line) : "")});
#else
    if(level != LogLvl::DBUG) ConsoleLog(std::string{type + msg + " : " + file + (line > 0 ? " : " + std::to_string(line) : "")});
#endif
}

// PRIVATES

void Logger::ConsoleLog(const std::string& msg){
    std::cout << msg << '\n';
}