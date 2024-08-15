#ifndef MIBAR_RANDR_H
#define MIBAR_RANDR_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <xcb/randr.h>

#include "logger.h"

struct dpyInfo{
    xcb_randr_get_crtc_info_reply_t* crtc = nullptr;
    int dpi = 0;
};

class Randr{
public:
    Randr(xcb_connection_t* c, xcb_screen_t* s);
    ~Randr();

    /**
        A function that returns crtc info for the display name 

        @param display This is the name of the display to get the information about

        @return Returns a pointer to the crtc info reply containing info such as scale and position
    */
    //const xcb_randr_get_crtc_info_reply_t* GetDisplayInfo(const std::string& display) const;
    //const xcb_randr_get_crtc_info_reply_t* GetPrimaryDisplay(xcb_connection_t* c, xcb_window_t w) const;

    const xcb_randr_get_crtc_info_reply_t* GetCrtcInfo(const std::string& display) const;
    const xcb_randr_get_crtc_info_reply_t* GetPrimaryDisplay(xcb_connection_t* c, xcb_window_t w) const;
    const int GetDisplayDPI(const std::string& display);
    const std::unique_ptr<dpyInfo> GetDisplayInfo(const std::string& display);

private:
    std::shared_ptr<Logger> m_logger;

    //std::unordered_map<std::string, xcb_randr_get_crtc_info_reply_t*> m_displays;
    std::unordered_map<std::string, dpyInfo> m_displays;
};

#endif