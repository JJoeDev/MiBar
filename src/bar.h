#ifndef MIBAR_BAR_H
#define MIBAR_BAR_H

#include <xcb/xcb.h>

#include "logger.h"

class mibar{
public:
    mibar();
    ~mibar();

    /**
        Run the XCB event loop. This loop waits for exposure events to know when to re-render the bar
    */
    void EventLoop();

private:
    // Basic connections
    xcb_connection_t* m_conn = nullptr;
    xcb_screen_t* m_screen = nullptr;
    xcb_window_t m_window = 0;

    int m_x, m_y, m_w, m_h;

    uint32_t m_winMask = 0;
    uint32_t m_winValues[2];

    Logger m_logger;
};

#endif