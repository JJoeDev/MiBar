#include <cassert>
#include <xcb/xcb.h>

#include "renderer.h"

Renderer::Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w) : m_conn(c), m_window(w){
    

    m_drawGC = xcb_generate_id(c);
    m_clearGC = xcb_generate_id(c);
    m_underlineGC = xcb_generate_id(c);

    m_font = xcb_generate_id(c);
    xcb_open_font_checked(c, m_font, 5, "fixed");

    xcb_query_font_reply_t* fontInfo = xcb_query_font_reply(c, xcb_query_font(c, m_font), nullptr);
    m_charWidth = fontInfo->max_bounds.character_width;

    m_logger.Log("", 0, "fixed font char width: " + std::to_string(m_charWidth), LogLvl::DBUG);

    xcb_create_gc(c, m_drawGC, w, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, (uint32_t[]){m_palette[1], m_palette[0], m_font});
    xcb_create_gc(c, m_clearGC, w, XCB_GC_FOREGROUND, (uint32_t[]){m_palette[0]});
    xcb_create_gc(c, m_underlineGC, w, XCB_GC_FOREGROUND, (uint32_t[]){m_palette[2]});

    free(fontInfo);
    
    xcb_close_font_checked(m_conn, m_font);
    xcb_flush(c);
}

Renderer::~Renderer(){
    m_components.clear();
}

void Renderer::DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, xcb_gcontext_t& gc){
    xcb_poly_fill_rectangle(m_conn, m_window, gc, 1, (const xcb_rectangle_t[]){{x, y, w, h}});
}

void Renderer::Clear(int16_t x, int16_t y, uint16_t w, uint16_t h){
    DrawRect(x, y, w, h, m_clearGC);
}

void Renderer::DrawComponents(const std::string& str, const int16_t x, const int16_t y){
    xcb_get_geometry_reply_t* geom = xcb_get_geometry_reply(m_conn, xcb_get_geometry(m_conn, m_window), nullptr);
    
    for(auto&& component : m_components){
        component->Draw(m_conn, m_window, m_drawGC);

        if(!ENABLE_UNDERLINE) continue;

        DrawRect(component->x, geom->height - UNDERLINE_HEIGHT, component->GetWidth(m_charWidth), UNDERLINE_HEIGHT, m_underlineGC);
    }

    free(geom);
}

void Renderer::SetForeground(uint32_t idx){
    assert(idx <= m_palette.size());
    xcb_change_gc(m_conn, m_drawGC, XCB_GC_FOREGROUND, (const uint32_t[]){m_palette[idx]});
}

void Renderer::SetBackground(uint32_t idx){
    assert(idx <= m_palette.size());
    xcb_change_gc(m_conn, m_drawGC, XCB_GC_BACKGROUND, (const uint32_t[]){m_palette[idx]});
    xcb_change_gc(m_conn, m_clearGC, XCB_GC_FOREGROUND, (const uint32_t[]){m_palette[idx]});
}

void Renderer::SetUnderline(uint32_t idx){
    assert(idx <= m_palette.size());
    xcb_change_gc(m_conn, m_underlineGC, XCB_GC_FOREGROUND, (const uint32_t[]){m_palette[idx]});
}

// PRIVATE

void Renderer::DrawUnderline(const xcb_rectangle_t& rect){
    xcb_poly_fill_rectangle(m_conn, m_window, m_underlineGC, 1, (const xcb_rectangle_t[]){rect});
}