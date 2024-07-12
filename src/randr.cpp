#include <iostream>

#include "randr.h"

Randr::Randr(xcb_connection_t* c, xcb_screen_t* s){
    xcb_randr_get_screen_resources_current_reply_t* resReply;
    xcb_randr_output_t* outputs;
    int num;

    resReply = xcb_randr_get_screen_resources_current_reply(c, xcb_randr_get_screen_resources_current(c, s->root), nullptr);

    if(!resReply){
        std::cerr << "[ERROR] failed to fetch current randr screen resources!\n";
        return;
    }

    num = xcb_randr_get_screen_resources_current_outputs_length(resReply);
    outputs = xcb_randr_get_screen_resources_current_outputs(resReply);

    if(num < 1){ // we have less than one output? I don't think so
        free(resReply);
        return;
    }

    for(int i = 0; i < num; ++i){
        xcb_randr_get_output_info_reply_t* infoReply;

        infoReply = xcb_randr_get_output_info_reply(c, xcb_randr_get_output_info(c, outputs[i], XCB_CURRENT_TIME), nullptr);

        if(!infoReply || infoReply->crtc == XCB_NONE || infoReply->connection != XCB_RANDR_CONNECTION_CONNECTED){
            std::cerr << "[ERROR] failed getting output information!\n";
            free(infoReply);
            continue;
        }

        xcb_randr_get_crtc_info_reply_t* ciReply = xcb_randr_get_crtc_info_reply(c, xcb_randr_get_crtc_info(c, infoReply->crtc, XCB_CURRENT_TIME), nullptr);

        if(!ciReply){
            std::cerr << "[ERROR] failed to get RandR crtc information!\n";
            free(resReply);
            continue;
        }

        std::string displayName(reinterpret_cast<char*>(xcb_randr_get_output_info_name(infoReply)), xcb_randr_get_output_info_name_length(infoReply));

        std::cout << "[INFO] Display with name: " << displayName << " found and added to map!\n";
        m_displays[displayName] = ciReply;
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
    for(const auto& it : m_displays){
        if(it.first == display){
            std::cout << "[INFO] Found display with name: " << display << '\n';
            return it.second;
        }
    }

    std::cerr << "[ERROR] Could not find display with name: " << display << '\n';
    return nullptr;
}