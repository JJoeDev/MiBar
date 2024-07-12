#ifndef MIBAR_BAR_H
#define MIBAR_BAR_H

#include <xcb/xcb.h>

class mibar{
public:
    mibar();
    ~mibar();

    void EventLoop();

private:
    // Basic connections
    xcb_connection_t* m_conn = nullptr;
    xcb_screen_t* m_screen;
    xcb_window_t m_window = 0;

    uint32_t m_winMask;
    uint32_t m_winValues[2];
};

#endif