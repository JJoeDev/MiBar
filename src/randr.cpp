#include "randr.h"

#include <limits>

Randr::Randr(xcb_connection_t* c, xcb_screen_t* s){
    xcb_randr_get_screen_resources_current_reply_t* resReply;
    xcb_randr_output_t* outputs;
    int num;

    resReply = xcb_randr_get_screen_resources_current_reply(c, xcb_randr_get_screen_resources_current(c, s->root), nullptr);

    if(!resReply){
        m_logger.Log(__FILE_NAME__, __LINE__, "failed to fetch current randr screen resources!", LogLvl::ERROR);
        return;
    }

    num = xcb_randr_get_screen_resources_current_outputs_length(resReply);
    outputs = xcb_randr_get_screen_resources_current_outputs(resReply);

    if(num < 1){ // we have less than one output? I don't think so
        m_logger.Log(__FILE_NAME__, 0, "XCB Randr could not find any monitors", LogLvl::WARNING);
        free(resReply);
        return;
    }

    for(int i = 0; i < num; ++i){
        xcb_randr_get_output_info_reply_t* infoReply;
        infoReply = xcb_randr_get_output_info_reply(c, xcb_randr_get_output_info(c, outputs[i], XCB_CURRENT_TIME), nullptr);

        if(!infoReply || infoReply->crtc == XCB_NONE || infoReply->connection != XCB_RANDR_CONNECTION_CONNECTED){
            m_logger.Log(__FILE_NAME__, __LINE__, "XCB Output information not available for this display!", LogLvl::WARNING);
            free(infoReply);
            continue;
        }

        xcb_randr_get_crtc_info_reply_t* crtcReply = xcb_randr_get_crtc_info_reply(c, xcb_randr_get_crtc_info(c, infoReply->crtc, XCB_CURRENT_TIME), nullptr);

        if(!crtcReply){
            m_logger.Log(__FILE_NAME__, __LINE__, "Could not get crtc reply!", LogLvl::ERROR);
            free(resReply);
            continue;
        }

        std::string displayName(reinterpret_cast<char*>(xcb_randr_get_output_info_name(infoReply)), xcb_randr_get_output_info_name_length(infoReply));

        m_logger.Log("", 0, "Display with name: " + displayName + " added to map");
        m_displays[displayName] = crtcReply;
        free(infoReply);
    }
    free(resReply);
}

Randr::~Randr(){
    for(auto& i : m_displays){
        free(i.second);
    }
    m_displays.clear();
}

const xcb_randr_get_crtc_info_reply_t* Randr::GetDisplayInfo(const std::string& display) const {
    for(const auto& [name, crtc] : m_displays){
        if(name == display){
            return crtc;
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

    for(const auto& [name, crtc] : m_displays){
        int x = crtc->x;
        int y = crtc->y;

        auto m = crtc->mode;
        

        if(x < minX || (x == minX && y < minY)){
            minX = x;
            minY = y;
            monitor = m_displays.at(name);
        }
    }

    if(monitor){
        return monitor;
    }

    return nullptr;
}