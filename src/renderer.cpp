#include <algorithm>
#include <cassert>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>

#include "renderer.h"

Renderer::Renderer(const xcb_screen_t* s, xcb_connection_t* c, xcb_window_t& w) : m_conn(c), m_window(w){
    // Font loading
    m_font = xcb_generate_id(c);

    xcb_void_cookie_t fontCookie = xcb_open_font_checked(c, m_font, strlen(FONT), FONT);
    if(!TestCookie(fontCookie, c)){
        m_logger.Log(__FILE_NAME__, __LINE__, "Could not open user-defined font. Trying Fallback font", LogLvl::WARNING);
        xcb_open_font_checked(c, m_font, strlen(FONT_FALLBACK), FONT_FALLBACK);
    }

    // Initialize graphic contexts
    m_drawGC = xcb_generate_id(c);
    m_clearGC = xcb_generate_id(c);
    m_underlineGC = xcb_generate_id(c);

    xcb_create_gc(c, m_drawGC, w, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT, (const uint32_t[]){m_palette[1], m_palette[0], m_font});
    xcb_create_gc(c, m_clearGC, w, XCB_GC_FOREGROUND, (const uint32_t[]){m_palette[0]});
    xcb_create_gc(c, m_underlineGC, w, XCB_GC_FOREGROUND, (const uint32_t[]){m_palette[4]});

    // Get Window Geometry
    m_geometry = xcb_get_geometry_reply(c, xcb_get_geometry(c, w), nullptr);
}

Renderer::~Renderer(){
    m_geometry = nullptr;
    xcb_close_font_checked(m_conn, m_font);
}

void Renderer::DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, xcb_gcontext_t& gc){
    xcb_poly_fill_rectangle(m_conn, m_window, gc, 1, (const xcb_rectangle_t[]){{x, y, w, h}});
}

void Renderer::DrawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, int idx){
    SetForeground(4);
    printf("COORDS X: %i | Y: %i | W: %i | H: %i\n", x, y, w, h);
    DrawRect(x, y, w, h, m_drawGC);
}

void Renderer::Clear(int16_t x, int16_t y, uint16_t w, uint16_t h){
    DrawRect(x, y, w, h, m_clearGC);
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

void Renderer::DrawStr(const std::string& str, int x){
    int width = 0;
    int height = 0;
    
    auto size = GetStringSize(str);
    if(size.has_value()){
        std::tie(width, height) = size.value();
    }

    xcb_image_text_8(m_conn, str.length(), m_window, m_drawGC, x, BAR_HEIGHT / 2 + height / 2, str.c_str());
    DrawUnderline((const xcb_rectangle_t){(int16_t)x, BAR_HEIGHT - UNDERLINE_HEIGHT, static_cast<uint16_t>(width), BAR_HEIGHT});
}

// PRIVATE

// This code saved me ngl (https://lists.freedesktop.org/archives/xcb/2009-April/004611.html)
xcb_char2b_t* Renderer::BuildChar2b_t(const char* str, size_t length){
    xcb_char2b_t* result = (xcb_char2b_t*)malloc(length * sizeof(xcb_char2b_t));
    if(!result) return nullptr;
    
    std::transform(str, str + length, result, [](char c){
        return xcb_char2b_t{0, static_cast<uint8_t>(c)};
    });

    return result;
}

std::optional<std::pair<int, int>> Renderer::GetStringSize(const std::string& str){
    xcb_char2b_t* xcb_str = BuildChar2b_t(str.c_str(), str.length());

    xcb_query_text_extents_reply_t* reply;
    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(m_conn, m_font, str.length(), xcb_str);
    reply = xcb_query_text_extents_reply(m_conn, cookie, nullptr);

    if(reply == nullptr){
        m_logger.Log(__FILE_NAME__, __LINE__, "Reply got an error!", LogLvl::ERROR);
        return std::nullopt;
    }

    std::pair<int, int> size;
    size.first = reply->overall_width;
    size.second = reply->overall_ascent + reply->overall_descent;

    free(reply);
    free(xcb_str);

    return size;
}

void Renderer::DrawUnderline(const xcb_rectangle_t& rect){
    xcb_poly_fill_rectangle(m_conn, m_window, m_underlineGC, 1, (const xcb_rectangle_t[]){rect});
}