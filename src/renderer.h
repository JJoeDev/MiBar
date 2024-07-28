#ifndef MIBAR_RENDERER_H
#define MIBAR_RENDERER_H

#include <string>
#include <vector>

#include <xcb/xcb.h>

#include "logger.h"
#include "general.config.h"

#include "components/component.h"

class Renderer{
public:
    Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w);
    ~Renderer();

    /**
        Clear draws a rectangle with the BACKGROUND color over the whole bar
        @p int16_t x is the start X coordinate for the first corner
     */
    void Clear(int16_t x, int16_t y, uint16_t w, uint16_t h);
    void DrawText(const std::string& str, const int16_t x, const int16_t y, const bool autoCenterY);
    inline void AddComponent(std::unique_ptr<Component> c){m_components.emplace_back(c);}

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

    std::vector<std::unique_ptr<Component>> m_components;
};

#endif