#ifndef MIBAR_RENDERER_H
#define MIBAR_RENDERER_H

#include <xcb/xcb.h>

#include "configManager.h"

class Renderer{
public:
    Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w);
    ~Renderer();

    void DrawText(const std::string& str);

private:
    xcb_connection_t* m_conn;
    xcb_window_t& m_window;

    uint32_t drawPalette[2] = {0x000000, 0xFFFFFF};

    xcb_gcontext_t m_drawGC;
    xcb_gcontext_t m_clearGC;
    xcb_gcontext_t m_underlineGC;
};

#endif