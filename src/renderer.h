#ifndef MIBAR_RENDERER_H
#define MIBAR_RENDERER_H

#include <string>
#include <vector>
#include <array>

#include <xcb/xcb.h>

#include "logger.h"
#include "general.config.h"

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
        @param gc Use a custom graphics context to draw with
    */
    void DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, xcb_gcontext_t& gc);

    /**
        Calls DrawRect but uses the default background graphics context

        @param x Indicates the beginning X coordinate
        @param y Indicates the beginning Y coordinate
        @param w Indicates how many pixels wide the rectangle is
        @param h Indicates how many pixels high the rectangle is
    */
    void Clear(int16_t x, int16_t y, uint16_t w, uint16_t h);

    /**
        A function to set the foreground color to a new color from the palette

        @param idx The index of the palette the foreground color should be set to
    */
    void SetForeground(uint32_t idx);

    /**
        A function to set the background color to a new color from the palette

        @param idx The index of the palette the background color should be set to
    */
    void SetBackground(uint32_t idx);

    /**
        A function to set the underline color to a new color from the palette

        @param idx The index of the palette the underline color should be set to
    */
    void SetUnderline(uint32_t idx);

    void DrawStr(const char* str, int len);

private:
    xcb_connection_t* m_conn = nullptr;
    xcb_window_t& m_window;
    Logger m_logger;

    xcb_get_geometry_reply_t* m_geometry = nullptr;

    void DrawUnderline(const xcb_rectangle_t& rect);

    std::array<uint32_t, 5> m_palette = {BACKGROUND, FOREGROUND, COLOR1, COLOR2, COLOR3};

    xcb_font_t m_font = 0;
    int m_charWidth = 0;
    int m_charHeight = 0;

    xcb_gcontext_t m_drawGC = 0;
    xcb_gcontext_t m_clearGC = 0;
    xcb_gcontext_t m_underlineGC = 0;
};

#endif