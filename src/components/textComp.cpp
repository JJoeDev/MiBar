#include "textComp.h"

TextComponent::TextComponent(const std::string& str) : m_str(str){
    

    m_logger.Log(__FILE_NAME__, 0, "Text component constructor has run!", LogLvl::DBUG);
}

TextComponent::~TextComponent(){
    m_logger.Log("", 0, "Text component deconstructor has run!", LogLvl::DBUG);
}

void TextComponent::Draw(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc){
    xcb_image_text_8(conn, m_str.length(), window, gc, x, y, m_str.c_str());

    m_logger.Log("", 0, "Text component draw call with string: " + m_str, LogLvl::DBUG);
}

void TextComponent::Update(){

}

const int TextComponent::GetWidth(const int& fontWidth){
    return fontWidth * m_str.length();
}