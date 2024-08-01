#ifndef MIBAR_TEXT_COMPONENT_H
#define MIBAR_TEXT_COMPONENT_H

#include "component.h"

class TextComponent : public Component{
public:
    TextComponent(const std::string& str);
    ~TextComponent();

    void Draw(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc) override;
    void Update() override;

    const int GetWidth(const int& fontWidth) override;

private:
    Logger m_logger;

    const std::string m_str;
};

#endif