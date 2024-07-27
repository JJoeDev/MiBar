#include <xcb/xcb.h>

#include "renderer.h"

Renderer::Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w) : m_conn(c), m_window(w){
    logger = Logger::GetInstance();

    m_drawGC = xcb_generate_id(c);
    m_clearGC = xcb_generate_id(c);
    m_underlineGC = xcb_generate_id(c);

    xcb_create_gc(c, m_drawGC, w, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND, (uint32_t[]){drawPalette[1], drawPalette[0]});
    xcb_create_gc(c, m_clearGC, w, XCB_GC_FOREGROUND, (uint32_t[]){drawPalette[0]});

    xcb_flush(c);
}

Renderer::~Renderer(){

}

void Renderer::Clear(int16_t x, int16_t y, uint16_t w, uint16_t h){
    xcb_poly_fill_rectangle(m_conn, m_window, m_clearGC, 1, (const xcb_rectangle_t[]){{x, y, w, h}});
}

void Renderer::DrawText(const std::string& txt, int16_t x){
#if AUTO_CENTER_Y == 1
    xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(m_conn, xcb_get_geometry(m_conn, m_window), nullptr);
    if(geom){
        xcb_image_text_8(m_conn, txt.length(), m_window, m_drawGC, x, geom->height / 2 + 4, txt.c_str());
    }
    else{
        logger->Log(__FILE_NAME__, __LINE__, "Unable to get window geometry!", LogLvl::ERROR);
    }

    free(geom);
#else
    xcb_image_text_8(m_conn, txt.length(), m_window, m_drawGC, x, CUSTOM_Y, txt.c_str());
#endif
}