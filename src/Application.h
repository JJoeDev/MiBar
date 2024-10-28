#ifndef MIBAR_APPLICATION_H
#define MIBAR_APPLICATION_H

#include <memory>

#include <xcb/xcb.h>
#include <xcb/xproto.h>

namespace bar{
    class Application{
    public:
        Application();
        ~Application();

    private:
        std::shared_ptr<xcb_connection_t> m_conn{nullptr};
        std::unique_ptr<xcb_screen_t> m_screen{nullptr};
    };
}

#endif
