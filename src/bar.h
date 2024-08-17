#ifndef MIBAR_BAR_H
#define MIBAR_BAR_H

#include <xcb/xcb.h>

#include "logger.h"
#include "configParser.h"

class MiBar{
public:
    MiBar(const std::string& file);
    ~MiBar();

    /**
        Run the XCB event loop. This loop waits for exposure events to know when to re-render the bar
    */
    void EventLoop();

private:
    void SetProps();

    ConfigParser m_cfg;

    // Basic connections
    xcb_connection_t* m_conn = nullptr;
    xcb_screen_t* m_screen = nullptr;
    xcb_window_t m_window = 0;

    int m_configX = 0;
    int m_configY = 0;
    int m_configW = 0;
    int m_configH = 0;

    int m_x, m_y, m_w, m_h;

    uint32_t m_winMask = 0;
    uint32_t m_winValues[2];

    std::shared_ptr<Logger> m_logger;
};

#endif