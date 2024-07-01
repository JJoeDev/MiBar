#ifndef BAR_H
#define BAR_H

#include <cstdint>
#include <memory>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "configManager.h"

class Bar{
public:
    Bar();
    virtual ~Bar();

    void Run();

private:
    void LoadConfig();
    uint32_t HexStrToUint32(const std::string& str);

    bool ChangeWindowBG(const uint32_t& col);
    bool ChangeWindowSizePos();

    xcb_atom_t GetAtom(const char* atomName);
    
    xcb_connection_t* m_conn = nullptr;
    xcb_window_t m_window = {};
    
    unsigned int m_barX = 0;
    unsigned int m_barY = 0;
    unsigned int m_barW = 100;
    unsigned int m_barH = 20;

    uint32_t m_BG_COL = 0x000000; // 0xRR-GG-BB-AA
    uint32_t m_FG_COL = 0xFFFFFF; // 0xRR-GG-BB-AA

    xcb_colormap_t m_colMap;

    xcb_gcontext_t m_gc = {};
    uint32_t m_gcVal[2] = {};
    const char* m_gcDemoText = "Hello, World!";

    uint32_t m_valueMask = 0;
    uint32_t m_valueList[2] = {};

    const char* m_BAR_TITLE = "MiBar";

    cfgManager m_cfgMgr;
};

#endif
