#include "icccm.h"
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>

namespace icccmUtil{
    void SetWMName(xcb_connection_t* conn, xcb_window_t window, const std::string& wmName, const std::string& wmClass){
        xcb_icccm_set_wm_name(conn, window, XCB_ATOM_STRING, 8, static_cast<uint32_t>(wmName.length()), wmName.c_str());
        xcb_icccm_set_wm_class(conn, window, static_cast<uint32_t>(wmClass.length()), wmClass.c_str());
    }

    void SetWMSizeHints(xcb_connection_t* conn, xcb_window_t window, int x, int y, int width, int height){
        xcb_size_hints_t sizeHints{};

        xcb_icccm_size_hints_set_size(&sizeHints, false, width, height);
        xcb_icccm_size_hints_set_position(&sizeHints, false, x, y);

        xcb_icccm_size_hints_set_min_size(&sizeHints, width, height);
        xcb_icccm_size_hints_set_max_size(&sizeHints, width, height);
        xcb_icccm_size_hints_set_base_size(&sizeHints, width, height);

        xcb_icccm_set_wm_size_hints(conn, window, XCB_ATOM_WM_NORMAL_HINTS, &sizeHints);
    }
}
