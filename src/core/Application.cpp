#include <cairo/cairo-xcb.h>
#include <cairo/cairo.h>

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
        xcb_visualtype_t* visual_type = nullptr;
        xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(m_conn.get()));

        for(; screen_iter.rem; xcb_screen_next(&screen_iter)){
            xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
            for(; depth_iter.rem; xcb_depth_next(&depth_iter)){
                xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
                for(; visual_iter.rem; xcb_visualtype_next(&visual_iter)){
                    if(m_screen->root_visual == visual_iter.data->visual_id){
                        visual_type = visual_iter.data;
                        goto visual_found;
                    }
                }
            }
        }

visual_found: ;

        cairo_surface_t* surface = cairo_xcb_surface_create(m_conn.get(), m_window, visual_type, 150, 150);
        cairo_t* cr = cairo_create(surface);

        cairo_set_source_rgb(cr, 0, 1, 0);
        cairo_paint(cr);

        cairo_set_source_rgb(cr, 1, 0, 0);
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, 150, 0);
        cairo_line_to(cr, 150, 150);
        cairo_close_path(cr);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 1);
        cairo_set_line_width(cr, 20);
        cairo_move_to(cr, 0, 150);
        cairo_line_to(cr, 150, 0);
        cairo_stroke(cr);

        cairo_surface_flush(surface);

        miUtil::Logger::INFO("Clear function has run");
    }
}
