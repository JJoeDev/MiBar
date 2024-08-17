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
        Get a pointer to xcb_randr_get_crtc_info_reply_t for a given display

        @param display name of the display to get a crtc pointer to
        @return xcb_randr_get_crtc_info_reply_t*
    */
    const xcb_randr_get_crtc_info_reply_t* GetCrtcInfo(const std::string& display) const;

    /**
        Get a pointer to the display at x = 0, y = 0 (primary implementation in the works)

        @param c a pointer to the xcb_connection_t
        @param w a xcb_window_t
        @return xcb_randr_get_crtc_info_reply_t*
    */
    const xcb_randr_get_crtc_info_reply_t* GetPrimaryDisplay(xcb_connection_t* c, xcb_window_t w) const;

    /**
        Get a calculated DPI value for a given display

        @param display name of the display to get the DPI from
        @return const int : DPI value
    */
    const int GetDisplayDPI(const std::string& display);

    /**
        Get all stored data for a given display

        @param display name of the display to get data from
        @return std::unique_ptr to a dpyInfo struct
    */
    const std::unique_ptr<dpyInfo> GetDisplayInfo(const std::string& display);

private:
    std::shared_ptr<Logger> m_logger;

    std::unordered_map<std::string, dpyInfo> m_displays;
};

#endif