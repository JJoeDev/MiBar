#include "Application.h"
#include "Logger.h"

namespace bar{
    Application::Application(const AppParams& params){
        xcb_connection_t* connection = xcb_connect(nullptr, nullptr); // display_name | screenp
        if(xcb_connection_has_error(connection)){
            miUtil::Logger::FATAL("XCB Connection Failed!");
        }

        // Add connection to shared pointer and set custom deleter
        m_conn.reset(connection, [](xcb_connection_t* connection){
            xcb_disconnect(connection);
            miUtil::Logger::INFO("XCB has disconnected!");
        });

        const xcb_setup_t* setup = xcb_get_setup(m_conn.get());
        xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
        m_screen = std::make_unique<xcb_screen_t>(*iter.data);

        xcb_gcontext_t foreground = xcb_generate_id(m_conn.get());
        uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
        uint32_t values[2] = {m_screen->white_pixel, 0};

        xcb_create_gc(m_conn.get(), foreground, m_window, mask, values);

        m_window = xcb_generate_id(m_conn.get());

        mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
        values[0] = m_screen->black_pixel;
        values[1] = XCB_EVENT_MASK_EXPOSURE;

        xcb_create_window(m_conn.get(),                // Connection
                        XCB_COPY_FROM_PARENT,          // Depth (same as root)
                        m_window,                        // Window ID
                        m_screen->root,                  // Parent Window
                        0, 0,                          // X, Y
                        150, 150,                      // Width, Height
                        10,                            // Border Width
                        XCB_WINDOW_CLASS_INPUT_OUTPUT, // Class
                        m_screen->root_visual,           // Visual
                        mask, values);                   // Masks, not in use yet

        xcb_icccm_set_wm_name(m_conn.get(), m_window, XCB_ATOM_STRING, 8, static_cast<uint32_t>(params.AppName.length()), params.AppName.c_str());

        xcb_map_window(m_conn.get(), m_window); // Map window on screen
        xcb_flush(m_conn.get()); // Flush all commands to xcb
    }

    Application::~Application(){
        xcb_destroy_window(m_conn.get(), m_window);
        miUtil::Logger::INFO("DECONSTRUCTOR");
    }
}
