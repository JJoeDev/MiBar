#include "logger.h"
#include "randr.h"

Randr::Randr(xcb_connection_t* c, xcb_screen_t* s){
    auto logger = Logger::GetInstance();

    xcb_randr_get_screen_resources_current_reply_t* resReply;
    xcb_randr_output_t* outputs;
    int num;

    resReply = xcb_randr_get_screen_resources_current_reply(c, xcb_randr_get_screen_resources_current(c, s->root), nullptr);

    if(!resReply){
        logger->Log(__FILE_NAME__, __LINE__, "failed to fetch current randr screen resources!", LogLvl::ERROR);
        return;
    }

    num = xcb_randr_get_screen_resources_current_outputs_length(resReply);
    outputs = xcb_randr_get_screen_resources_current_outputs(resReply);

    if(num < 1){ // we have less than one output? I don't think so
        logger->Log(__FILE_NAME__, 0, "XCB Randr could not find any monitors", LogLvl::WARNING);
        free(resReply);
        return;
    }

    for(int i = 0; i < num; ++i){
        xcb_randr_get_output_info_reply_t* infoReply;

        infoReply = xcb_randr_get_output_info_reply(c, xcb_randr_get_output_info(c, outputs[i], XCB_CURRENT_TIME), nullptr);

        if(!infoReply || infoReply->crtc == XCB_NONE || infoReply->connection != XCB_RANDR_CONNECTION_CONNECTED){
            logger->Log(__FILE_NAME__, __LINE__, "XCB Output information not available for this display!", LogLvl::WARNING);
            free(infoReply);
            continue;
        }

        xcb_randr_get_crtc_info_reply_t* ciReply = xcb_randr_get_crtc_info_reply(c, xcb_randr_get_crtc_info(c, infoReply->crtc, XCB_CURRENT_TIME), nullptr);

        if(!ciReply){
            logger->Log(__FILE_NAME__, __LINE__, "Could not get crtc reply!", LogLvl::ERROR);
            free(resReply);
            continue;
        }

        std::string displayName(reinterpret_cast<char*>(xcb_randr_get_output_info_name(infoReply)), xcb_randr_get_output_info_name_length(infoReply));

        logger->Log("", 0, "Display with name: " + displayName + " added to map");
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
            return it.second;
        }
    }

    Logger::GetInstance()->Log(__FILE_NAME__, __LINE__, "Could not find display with name: " + display, LogLvl::ERROR);
    return nullptr;
}