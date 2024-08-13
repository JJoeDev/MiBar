#ifndef MIBAR_RANDR_H
#define MIBAR_RANDR_H

#include <memory>
#include <unordered_map>

#include <xcb/randr.h>

#include "logger.h"

class Randr{
public:
    Randr(xcb_connection_t* c, xcb_screen_t* s);
    ~Randr();

    /**
        A function that returns crtc info for the display name 

        @param display This is the name of the display to get the information about

        @return Returns a pointer to the crtc info reply containing info such as scale and position
    */
    const xcb_randr_get_crtc_info_reply_t* GetDisplayInfo(const std::string& display) const;
    const xcb_randr_get_crtc_info_reply_t* GetPrimaryDisplay(xcb_connection_t* c, xcb_window_t w) const;

private:
    Logger m_logger;

    std::unordered_map<std::string, xcb_randr_get_crtc_info_reply_t*> m_displays;
};

#endif