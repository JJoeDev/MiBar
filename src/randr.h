#ifndef MIBAR_RANDR_H
#define MIBAR_RANDR_H

#include <memory>
#include <unordered_map>

#include <xcb/randr.h>

class Randr{
public:
    Randr(xcb_connection_t* c, xcb_screen_t* s);
    ~Randr();

    const xcb_randr_get_crtc_info_reply_t* GetDisplayInfo(const std::string& display) const;

    void Init();

private:
    std::unordered_map<std::string, xcb_randr_get_crtc_info_reply_t*> m_displays;
};

#endif