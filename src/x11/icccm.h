#ifndef MIBAR_ICCCM_H
#define MIBAR_ICCCM_H

#include <string>

#include <xcb/xcb_icccm.h>

namespace icccmUtil{
    void SetWMName(xcb_connection_t* conn, xcb_window_t window, const std::string& wmName, const std::string& wmClass);
    void SetWMSizeHints(xcb_connection_t* conn, xcb_window_t window, int x, int y, int width, int height);
}

#endif
