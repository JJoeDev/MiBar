#ifndef BASE_MODULE_H
#define  BASE_MODULE_H

#include <xcb/xcb.h>
#include <string>

class Bar;

class baseModule{
public:
    virtual ~baseModule() = default;
    virtual void Render(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc, int x, int y) = 0;
    virtual int GetWidth() const = 0;
};

#endif