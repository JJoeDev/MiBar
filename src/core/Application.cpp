#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_image.h>

#include "icccm.h"
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

        uint32_t mask[2] = {XCB_CW_BACK_PIXMAP, XCB_EVENT_MASK_EXPOSURE};

        m_window = xcb_generate_id(m_conn.get());
        xcb_create_window(m_conn.get(),                // Connection
                        XCB_COPY_FROM_PARENT,          // Depth (same as root)
                        m_window,                        // Window ID
                        m_screen->root,                  // Parent Window
                        20, 20,                          // X, Y
                        150, 150,                      // Width, Height
                        5,                            // Border Width
                        XCB_WINDOW_CLASS_INPUT_OUTPUT, // Class
                        m_screen->root_visual,           // Visual
                        XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK, mask);                   // Masks, not in use yet

        icccmUtil::SetWMName(m_conn.get(), m_window, params.AppName, params.AppName);

        xcb_map_window(m_conn.get(), m_window); // Map window on screen
        xcb_flush(m_conn.get()); // Flush all commands to xcb

        miUtil::Logger::INFO("CONSTRUCTOR HAS RUN");
    }

    Application::~Application(){
        xcb_destroy_window(m_conn.get(), m_window);
    }

    void Application::EventLoop(){
        xcb_generic_event_t* e = nullptr;
        while((e = xcb_wait_for_event(m_conn.get()))){
            switch(e->response_type & ~0x80){
            case XCB_EXPOSE:
                ClearBG();
                miUtil::Logger::INFO("EXPOSE EVENT");
                break;
            }

            free(e);
            xcb_flush(m_conn.get());
        }
    }

    void Application::ClearBG(){
        miUtil::Logger::INFO("Clear function has run");
    }
}
