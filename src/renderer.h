#ifndef MIBAR_RENDERER_H
#define MIBAR_RENDERER_H

#include <string>

#include <xcb/xcb.h>

#include "logger.h"
#include "general.config.h"

class Renderer{
public:
    Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w);
    ~Renderer();

    // Drawing / Rendering
    void Clear(int16_t x, int16_t y, uint16_t w, uint16_t h);
    void DrawText(const std::string& str, const int16_t x, const int16_t y, const bool autoCenterY);

private:
    xcb_connection_t* m_conn;
    xcb_window_t& m_window;
    std::shared_ptr<Logger> logger;

    void DrawUnderline(const xcb_rectangle_t& rect);

    uint32_t drawPalette[5] = {BACKGROUND, FOREGROUND, COLOR1, COLOR2, COLOR3};

    xcb_font_t m_font;
    int m_charWidth;

    xcb_gcontext_t m_drawGC;
    xcb_gcontext_t m_clearGC;
    xcb_gcontext_t m_underlineGC;
};

#endif