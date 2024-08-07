#include "renderer.h"
#include "utils.h"
#include "bar.h"
#include "randr.h"
#include "pluginManager.h"

#if AUTOMATIC_UPDATES_ENABLE
#include <thread>
#endif

mibar::mibar(){
    m_conn = xcb_connect(nullptr, nullptr);
    if(xcb_connection_has_error(m_conn)){
        m_logger.Log(__FILE_NAME__, __LINE__, "Could not connect to X server!", LogLvl::ERROR);
        return;
    }

    m_screen = xcb_setup_roots_iterator(xcb_get_setup(m_conn)).data;

    m_winMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    m_winValues[0] = m_screen->black_pixel;
    m_winValues[1] = XCB_EVENT_MASK_EXPOSURE;

    Randr r(m_conn, m_screen);
    const auto mon = r.GetDisplayInfo(TARGET_MONITOR);

    m_x = mon->x + BAR_X;
    m_y = mon->y + BAR_Y;
    m_w = mon->width + BAR_WIDTH;
    m_h = BAR_HEIGHT;

    m_window = xcb_generate_id(m_conn);
    xcb_create_window(m_conn,
                      m_screen->root_depth,
                      m_window,
                      m_screen->root,
                      m_x, m_y,
                      m_w, m_h,
                      0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      m_screen->root_visual,
                      m_winMask,
                      m_winValues);

    xcb_atom_t wmTypeAtom = GetAtom(m_conn, "_NET_WM_WINDOW_TYPE");
    xcb_atom_t wmTypeDockAtom = GetAtom(m_conn, "_NET_WM_WINDOW_TYPE_DOCK");
    xcb_change_property(m_conn, XCB_PROP_MODE_PREPEND, m_window, wmTypeAtom, XCB_ATOM_ATOM, 32, 1, &wmTypeDockAtom);

    // EWMH (Extended Window Manager Hint) Reserve space for bar
    xcb_atom_t wmStrutPartialAtom = GetAtom(m_conn, "_NET_WM_STRUT_PARTIAL");
    uint32_t strut[12] = {0}; // https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html

    xcb_change_property(m_conn, XCB_PROP_MODE_REPLACE, m_window, wmStrutPartialAtom, XCB_ATOM_CARDINAL, 32, 12, strut);

    // Set window title
    xcb_change_property(m_conn, XCB_PROP_MODE_REPLACE, m_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, sizeof(char*), 5, "MiBar"); // 5 = 5 chars

    xcb_map_window(m_conn, m_window);
    xcb_flush(m_conn);
}

mibar::~mibar(){
    
}

void mibar::EventLoop() {
    Renderer r(m_screen, m_conn, m_window);

    PluginManager pmgr;
    pmgr.ExposeFuncToLua("DrawString", [&r](const std::string& str, ALIGNMENT align, const int x){r.DrawStr(str, align, x);});
    pmgr.ExposeFuncToLua("DrawRect", [&r](int x, int y, int w, int h, int idx){r.DrawRect(x, y, w, h, idx);});

#if AUTOMATIC_UPDATES_ENABLE
    using namespace std::chrono;

    auto last_update = steady_clock::now();
    const auto update_interval = seconds(UPDATE_TIME);
#endif

    xcb_generic_event_t* e;
    while (true) {
        if ((e = xcb_poll_for_event(m_conn))) {
            if (e) {
                switch (e->response_type & 0x7F) {
                case XCB_EXPOSE:
                    r.Clear(0, 0, m_w, m_h);
                    pmgr.RunScripts();
                    xcb_flush(m_conn);
                    break;
                default:
                    // Handle other events if needed
                    break;
                }
                free(e);
            }
        }
#if AUTOMATIC_UPDATES_ENABLE
        else {
            auto now = steady_clock::now();
            if (now - last_update >= update_interval) {
                // Force update
                r.Clear(0, 0, m_w, m_h);
                pmgr.RunScripts();
                xcb_flush(m_conn);
                last_update = now;
            }
            // Yield CPU time to other threads
            std::this_thread::yield();
        }
#endif
    }
}