#ifndef MIBAR_CONFIG_PARSER_H
#define MIBAR_CONFIG_PARSER_H

#include <unordered_map>

#include "logger.h"

enum CFG_TYPE {
    BG_COL = 0,
    FG_COL,
    COL1,
    COL2,
    COL3,
    TARGET_MON,
    FONT,
    FB_FONT,
    BAR_W,
    BAR_H,
    BAR_X,
    BAR_Y,
    USE_UNDERL,
    UNDERL_H,
    UNDERL_OFF_X,
    UNDERL_OFF_Y,
    CFG_COUNT,
};

class ConfigParser{
public:
    ConfigParser();
    ~ConfigParser() = default;

    void Parse(const std::string& file = "config");

    const std::string GetConfig(const int key) const;

private:
    std::shared_ptr<Logger> m_logger;

    std::unordered_map<std::string, std::string> m_configs;

    const char* m_configTable[CFG_TYPE::CFG_COUNT]{
        "Background",
        "Foreground",
        "Color1",
        "Color2",
        "Color3",
        "TargetMonitor",
        "Font",
        "FontFallback",
        "BarWidth",
        "BarHeight",
        "BarX",
        "BarY",
        "UseUnderlines",
        "UnderlineHeight",
        "UnderlineOffsetX",
        "UnderlineOffsetY",
    };
};

#endif
