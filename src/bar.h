#ifndef BAR_H
#define BAR_H

#include <cstdint>
#include <memory>
#include <vector>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/randr.h>

#include "configManager.h"
#include "module.h"

class Bar{
public:
    Bar();
    virtual ~Bar();

    void Run();

private:
    void GetDisplays();

    void LoadConfig();
    void GetMonitorFromStrAndSetPos(const std::string& name);
    const uint32_t HexStrToUint32(const std::string& str) const;

    bool ChangeWindowBG(const uint32_t& col);
    bool ChangeWindowSizePos();

    void ModuleRender();

    xcb_atom_t GetAtom(const char* atomName);
    
    xcb_connection_t* m_conn = nullptr;
    xcb_screen_t* m_screen = nullptr;
    xcb_window_t m_window = {};

    const unsigned char m_numDisplays = 4;
    std::string m_monitorNames[4];
    xcb_randr_get_crtc_info_reply_t* m_crtcs[4];

    unsigned int m_barX = 0;
    unsigned int m_barY = 0;
    unsigned int m_barW = 100;
    unsigned int m_barH = 20;

    uint32_t m_BG_COL = 0x000000; // 0xRR-GG-BB-AA
    uint32_t m_FG_COL = 0xFFFFFF; // 0xRR-GG-BB-AA

    xcb_colormap_t m_colMap;

    xcb_gcontext_t m_gc = {};
    uint32_t m_gcVal[2] = {};

    uint32_t m_valueMask = 0;
    uint32_t m_valueList[2] = {};

    const char* m_BAR_TITLE = "MiBar";

    std::vector<baseModule*> m_modules;
    cfgManager m_cfgMgr;
};

#endif
