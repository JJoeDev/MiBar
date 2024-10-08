#include "randr.h"

#include <limits>

Randr::Randr(xcb_connection_t* c, xcb_screen_t* s){
    xcb_randr_get_screen_resources_current_reply_t* resReply;
    xcb_randr_output_t* outputs;
    int num;

    resReply = xcb_randr_get_screen_resources_current_reply(c, xcb_randr_get_screen_resources_current(c, s->root), nullptr);

    if(!resReply){
        m_logger->Log(MI_FILENAME, __LINE__, "Could not get current RandR screen resources!", LogLevel::ERROR);
        return;
    }

    num = xcb_randr_get_screen_resources_current_outputs_length(resReply);
    outputs = xcb_randr_get_screen_resources_current_outputs(resReply);

    if(num < 1){ // we have less than one output? I don't think so
        m_logger->Log(MI_FILENAME, __LINE__, "XCB RandR could not find any monitors!", LogLevel::WARNING);
        free(resReply);
        return;
    }

    for(int i = 0; i < num; ++i){
        xcb_randr_get_output_info_reply_t* infoReply;
        infoReply = xcb_randr_get_output_info_reply(c, xcb_randr_get_output_info(c, outputs[i], XCB_CURRENT_TIME), nullptr);

        if(!infoReply || infoReply->crtc == XCB_NONE || infoReply->connection != XCB_RANDR_CONNECTION_CONNECTED){
            m_logger->Log(MI_FILENAME, __LINE__, "Found display output but no connection", LogLevel::MESSAGE);
            free(infoReply);
            continue;
        }

        xcb_randr_get_crtc_info_reply_t* crtcReply = xcb_randr_get_crtc_info_reply(c, xcb_randr_get_crtc_info(c, infoReply->crtc, XCB_CURRENT_TIME), nullptr);

        if(!crtcReply){
            m_logger->Log(MI_FILENAME, __LINE__, "Could not get crtc reply from XCB!", LogLevel::ERROR);
            free(infoReply);
            continue;
        }

        // DPI = (THP / TW + TVP / TL) / 2
        int dpyDpi = crtcReply->width / infoReply->mm_width;
        m_logger->Log(MI_FILENAME, __LINE__, "DPI calculated to: " + std::to_string(dpyDpi), LogLevel::DEBUG);

        std::string displayName(reinterpret_cast<char*>(xcb_randr_get_output_info_name(infoReply)), xcb_randr_get_output_info_name_length(infoReply));

        m_logger->Log(MI_FILENAME, __LINE__, "ADDED " + displayName + "to map", LogLevel::INFO);
        m_displays[displayName] = {crtcReply, dpyDpi};

        free(infoReply);
    }
    free(resReply);
}

Randr::~Randr(){
    for(auto& [_, dpy] : m_displays){
        free(dpy.crtc);
    }
    m_displays.clear();
}

const xcb_randr_get_crtc_info_reply_t* Randr::GetCrtcInfo(const std::string& display) const {
    for(const auto& [name, dpy] : m_displays){
        if(name == display){
            return dpy.crtc;
        }
    }

    return nullptr;
}

const xcb_randr_get_crtc_info_reply_t* Randr::GetPrimaryDisplay(xcb_connection_t* c, xcb_window_t w) const {
    // xcb_randr_get_output_primary_reply_t* primary;
    // xcb_randr_get_output_primary_cookie_t cookie;

    // cookie = xcb_randr_get_output_primary(c, w);
    // primary = xcb_randr_get_output_primary_reply(c, xcb_randr_get_output_primary(c, w), nullptr);

    // if(!primary){ // !########## BUG: Somehow this always fails even though xrandr can see a primary display. I DO THE SAME AS i3 ON LINE 811 ##########!
    //     std::cerr << "COULD NOT GET PRIMARY REPLY!\n";
    //     free(primary);
    //     return nullptr;
    // }

    // xcb_randr_output_t output = primary->output;

    // xcb_randr_get_output_info_reply_t* infoReply;
    // infoReply = xcb_randr_get_output_info_reply(c, xcb_randr_get_output_info(c, output, XCB_CURRENT_TIME), nullptr);

    // if(!infoReply || infoReply->crtc == XCB_NONE || infoReply->connection != XCB_RANDR_CONNECTION_CONNECTED){
    //     std::cerr << "ERROR COULD NOT FIND PRIMARY DISPLAY!\n";
    //     free(infoReply);
    //     free(primary);
    //     return nullptr;
    // }

    // xcb_randr_get_crtc_info_reply_t* crtcReply = xcb_randr_get_crtc_info_reply(c, xcb_randr_get_crtc_info(c, infoReply->crtc, XCB_CURRENT_TIME), nullptr);

    // if(!crtcReply){
    //     std::cerr << "COULD NOT GET PRIMARY MONITOR!\n";
    //     free(primary);
    //     free(infoReply);
    //     free(crtcReply);
    //     return nullptr;
    // }

    // free(primary);
    // free(infoReply);

    // return crtcReply;

    xcb_randr_get_crtc_info_reply_t* monitor = nullptr;

    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();

    for(const auto& [name, dpy] : m_displays){
        int x = dpy.crtc->x;
        int y = dpy.crtc->y;

        if(x < minX || (x == minX && y < minY)){
            minX = x;
            minY = y;
            monitor = m_displays.at(name).crtc;
        }
    }

    if(monitor){
        return monitor;
    }

    m_logger->Log(MI_FILENAME, __LINE__, "Could not find any monitor as fallback. Please specify in config.bar", LogLevel::ERROR);

    return nullptr;
}

const int Randr::GetDisplayDPI(const std::string& display){
    for(const auto& [name, dpy] : m_displays){
        if(name == display){
            return dpy.dpi;
        }
    }

    m_logger->Log(MI_FILENAME, __LINE__, "Could not find a display to get dpi from!", LogLevel::ERROR);
    return 0;
}

const std::unique_ptr<dpyInfo> Randr::GetDisplayInfo(const std::string& display){
    for(const auto& [name, dpy] : m_displays){
        if(name == display){
            return std::make_unique<dpyInfo>(dpy);
        }
    }

    m_logger->Log(MI_FILENAME, __LINE__, "Could not get dpyInfo struct because no display was found!", LogLevel::ERROR);
    return nullptr;
}