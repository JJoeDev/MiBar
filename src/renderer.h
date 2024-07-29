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
        Draws a rectangle from x, y to w, h with the colot BACKGROUND from general.config.h

        @param x Indicates the beginning X coordinate
        @param y Indicates the beginning Y coordinate
        @param w Indicates how many pixels wide the rectangle is
        @param h Indicates how many pixels high the rectangle is
    */
    void Clear(int16_t x, int16_t y, uint16_t w, uint16_t h);

    /**
        TEMPORARY FUNCTION. WILL BE MOVED TO ITS OWN COMPONENT
    */
    void DrawText(const std::string& str, const int16_t x, const int16_t y, const bool autoCenterY);

    /**
        Will add an object that inherits Component to a vector so the renderer can render all components

        @param c Is a unique pointer to an object that inherits Component
    */
    inline void AddComponent(std::unique_ptr<Component> c){m_components.emplace_back(std::move(c));}

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