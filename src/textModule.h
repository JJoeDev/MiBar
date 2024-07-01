#ifndef TEXT_MODULE_H
#define TEXT_MODULE_H

#include "module.h"

class textModule : public baseModule{
public:
    textModule(const std::string& text);

    void Render(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc, int x, int y) override;
    int GetWidth() const override;

private:
    std::string m_txt;
};

#endif