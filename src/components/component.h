#ifndef MIBAR_COMPONENT_H
#define MIBAR_COMPONENT_H

#include <xcb/xcb.h>

class Component{
public:
    virtual ~Component();
    virtual void Draw(xcb_connection_t* conn, xcb_window_t& window, xcb_gcontext_t& gc) = 0;
    virtual void Update() = 0;
    virtual const int GetWidth() = 0;
};

#endif