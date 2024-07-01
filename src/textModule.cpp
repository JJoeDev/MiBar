#include "textModule.h"

textModule::textModule(const std::string& text) : m_txt(text){}

void textModule::Render(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc, int x, int y){
    xcb_image_text_8(conn, m_txt.length(), window, gc, x, y, m_txt.c_str());
}

int textModule::GetWidth() const{
    return m_txt.length() * 8;
}