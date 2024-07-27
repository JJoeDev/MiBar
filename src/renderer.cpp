#include <xcb/xcb.h>

#include "renderer.h"

Renderer::Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w) : m_conn(c), m_window(w){
    logger = Logger::GetInstance();

    m_drawGC = xcb_generate_id(c);
    m_clearGC = xcb_generate_id(c);
    m_underlineGC = xcb_generate_id(c);

    m_font = xcb_generate_id(c);
    xcb_open_font_checked(c, m_font, 5, "fixed");

    xcb_query_font_reply_t* fontInfo = xcb_query_font_reply(c, xcb_query_font(c, m_font), nullptr);
    m_charWidth = fontInfo->max_bounds.character_width;

    xcb_create_gc(c, m_drawGC, w, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, (uint32_t[]){drawPalette[1], drawPalette[0], m_font});
    xcb_create_gc(c, m_clearGC, w, XCB_GC_FOREGROUND, (uint32_t[]){drawPalette[0]});
    xcb_create_gc(c, m_underlineGC, w, XCB_GC_FOREGROUND, (uint32_t[]){drawPalette[2]});

    free(fontInfo);

    xcb_close_font_checked(m_conn, m_font);
    xcb_flush(c);
}

Renderer::~Renderer(){
}

void Renderer::Clear(int16_t x, int16_t y, uint16_t w, uint16_t h){
    xcb_poly_fill_rectangle(m_conn, m_window, m_clearGC, 1, (const xcb_rectangle_t[]){{x, y, w, h}});
}

void Renderer::DrawText(const std::string& str, const int16_t x, const int16_t y, const bool autoCenterY){
    xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(m_conn, xcb_get_geometry(m_conn, m_window), nullptr);

    if(autoCenterY){
        if(geom){
            xcb_image_text_8(m_conn, str.length(), m_window, m_drawGC, x, geom->height / 2 + 4, str.c_str());
        }
        else{
            logger->Log(__FILE_NAME__, __LINE__, "Unable to get window geometry!", LogLvl::ERROR);
        }

    }
    else {
        xcb_image_text_8(m_conn, str.length(), m_window, m_drawGC, x, y, str.c_str());
    }

    if(ENABLE_UNDERLINE){
        int16_t yPos = static_cast<int16_t>(geom->height - UNDERLINE_HEIGHT + UNDERLINE_Y_OFFSET);
        int16_t xPos = static_cast<int16_t>(x + UNDERLINE_X_OFFSET);

        uint16_t length = static_cast<uint16_t>(str.length() * m_charWidth);

        const auto rect = xcb_rectangle_t{xPos, yPos, length, UNDERLINE_HEIGHT};
        DrawUnderline(rect);
    }
    
    free(geom);
}

// PRIVATE

void Renderer::DrawUnderline(const xcb_rectangle_t& rect){
    xcb_poly_fill_rectangle(m_conn, m_window, m_underlineGC, 1, (const xcb_rectangle_t[]){rect});
}