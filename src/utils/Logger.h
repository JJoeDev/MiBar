#ifndef MIBAR_LOGGER_H
#define MIBAR_LOGGER_H

#include <iostream>
#include <unordered_map>

namespace miUtil{
    enum class Color{
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        RED_BG,
        YELLOW_BG,
        DEFAULT,
    };

    const static std::unordered_map<Color, std::string> ansiColor = {
        { Color::BLACK,     "\033[1;30m" },
        { Color::RED,       "\033[1;31m" },
        { Color::GREEN,     "\033[1;32m" },
        { Color::YELLOW,    "\033[1;33m" },
        { Color::BLUE,      "\033[1;34m" },
        { Color::MAGENTA,   "\033[1;35m" },
        { Color::CYAN,      "\033[1;36m" },
        { Color::WHITE,     "\033[1;37m" },
        { Color::RED_BG,    "\033[7;31m" },
        { Color::YELLOW_BG, "\033[7;33m" },
        { Color::DEFAULT,   "\033[0m"    },
    };

    class Logger{
    public:
        template<typename... Args>
        static void FATAL(Args&&... args){
            std::cerr << ansiColor.at(Color::RED_BG) << "[ FATAL ] " << ansiColor.at(Color::DEFAULT);
            (std::cerr << ... << args);
            std::cerr << std::endl;
            exit(-1);
        }

        template<typename... Args>
        static void INFO(Args&&... args){
            std::clog << ansiColor.at(Color::GREEN) << "[ INFO  ] " << ansiColor.at(Color::DEFAULT);
            (std::clog << ... << args);
            std::clog << std::endl;
        }
    };
}

#endif
