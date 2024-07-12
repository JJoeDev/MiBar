#include <xcb/xcb.h>

#include "renderer.h"

Renderer::Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w) : m_conn(c), m_window(w){
    m_drawGC = xcb_generate_id(c);
    xcb_create_gc(m_conn, m_drawGC, m_window, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND, &drawPalette);
    xcb_flush(c);
}

Renderer::~Renderer(){

}

void Renderer::DrawText(const std::string& txt){
    xcb_image_text_8(m_conn, txt.length(), m_window, m_drawGC, 10, 13, txt.c_str());
}