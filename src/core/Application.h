#ifndef MIBAR_APPLICATION_H
#define MIBAR_APPLICATION_H

#include <memory>
#include <string>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_icccm.h>

namespace bar{
    struct AppParams{
        std::string AppName{};
    };

    class Application{
    public:
        Application(const AppParams& params);
        ~Application();

        void EventLoop();

        void ClearBG();

    private:
        std::shared_ptr<xcb_connection_t> m_conn{nullptr};
        std::unique_ptr<xcb_screen_t> m_screen{nullptr};
        xcb_window_t m_window;
    };
}

#endif
