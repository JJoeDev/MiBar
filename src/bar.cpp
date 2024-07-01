#include <cstring>
#include <functional>
#include <iostream>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/randr.h>

#include "bar.h"
#include "configManager.h"

Bar::Bar(){    
    m_conn = xcb_connect(nullptr, nullptr);
    if(xcb_connection_has_error(m_conn)){
        std::cerr << "Connection to X11 server has failed!\n";
        return;
    }

    const xcb_setup_t* setup = xcb_get_setup(m_conn);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    m_screen = iter.data;

    GetDisplays();

    m_colMap = m_screen->default_colormap;

    m_window = xcb_generate_id(m_conn);

    m_valueMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    m_valueList[0] = m_screen->black_pixel;
    m_valueList[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(m_conn,
                      XCB_COPY_FROM_PARENT,
                      m_window,
                      m_screen->root,
                      m_barX,
                      m_barY,
                      m_barW,
                      m_barH,
                      0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      m_screen->root_visual,
                      m_valueMask,
                      m_valueList);

    xcb_atom_t wmTypeAtom = GetAtom("_NET_WM_WINDOW_TYPE");
    xcb_atom_t wmTypeDockAtom = GetAtom("_NET_WM_WINDOW_TYPE_DOCK");
    xcb_change_property(m_conn, XCB_PROP_MODE_REPLACE, m_window, wmTypeAtom, XCB_ATOM_ATOM, 32, 1, &wmTypeDockAtom);

    xcb_atom_t wmStrutPartialAtom = GetAtom("_NET_WM_STRUT_PARTIAL");
    uint32_t strut[12] = {0};
    strut[2] = m_barH;
    strut[8] = 0;
    strut[9] = m_barW - 1;

    xcb_change_property(m_conn, XCB_PROP_MODE_REPLACE, m_window, wmStrutPartialAtom, XCB_ATOM_CARDINAL, 32, 12, strut);
    xcb_change_property(m_conn, XCB_PROP_MODE_REPLACE, m_window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, sizeof(char*), std::strlen(m_BAR_TITLE), m_BAR_TITLE);

    LoadConfig();

    m_gcVal[0] = m_FG_COL;
    m_gcVal[1] = m_BG_COL;

    m_gc = xcb_generate_id(m_conn);
    xcb_create_gc(m_conn, m_gc, m_window, XCB_GC_FOREGROUND | XCB_GC_BACKGROUND, m_gcVal);
    
    xcb_map_window(m_conn, m_window);
    xcb_flush(m_conn);

}

Bar::~Bar(){
    for(auto& i : m_crtcs){
        delete i;
    }

    xcb_disconnect(m_conn);
}

void Bar::GetDisplays(){
    xcb_randr_get_screen_resources_current_cookie_t resCookie;
    xcb_randr_get_screen_resources_current_reply_t* resReply;
    xcb_randr_output_t* outputs;

    resCookie = xcb_randr_get_screen_resources_current(m_conn, m_screen->root);
    resReply = xcb_randr_get_screen_resources_current_reply(m_conn, resCookie, nullptr);

    if(!resReply){
        std::cerr << "Error getting screen resources\n";
        return;
    }

    int numOutputs = xcb_randr_get_screen_resources_current_outputs_length(resReply);
    outputs = xcb_randr_get_screen_resources_current_outputs(resReply);

    static int appendIndex = 0;

    for(int i = 0; i < numOutputs; ++i){
        xcb_randr_output_t outI = outputs[i];

        xcb_randr_get_output_info_cookie_t infoCookie;
        xcb_randr_get_output_info_reply_t* infoReply;

        infoCookie = xcb_randr_get_output_info(m_conn, outI, XCB_CURRENT_TIME);
        infoReply = xcb_randr_get_output_info_reply(m_conn, infoCookie, nullptr);

        if(!infoReply){
            std::cerr << "Error getting output info!\n";
            return;
        }

        std::string name(reinterpret_cast<char*>(xcb_randr_get_output_info_name(infoReply)), xcb_randr_get_output_info_name_length(infoReply));

        std::cout << "[XCB RANDR]\t\tOutput: " << name << '\n';

        if(infoReply->crtc != XCB_NONE){
            xcb_randr_get_crtc_info_cookie_t crtcCoockie;
            xcb_randr_get_crtc_info_reply_t* crtcReply;

            crtcCoockie = xcb_randr_get_crtc_info(m_conn, infoReply->crtc, XCB_CURRENT_TIME);
            crtcReply = xcb_randr_get_crtc_info_reply(m_conn, crtcCoockie, nullptr);

            if(!crtcReply){
                std::cout << "Error getting CRTC information\n";
                free(infoReply);
                continue;
            }

            printf("[INFO XCB RANDR]\tPOSITION (%i, %i)\n", crtcReply->x, crtcReply->y);
            printf("[INFO XCB RANDR]\tSIZE (%i, %i)\n\n", crtcReply->width, crtcReply->height);

            if(appendIndex < m_numDisplays){
                m_crtcs[appendIndex] = crtcReply;
                m_monitorNames[appendIndex] = name;
                appendIndex++;
            }
            else{
                free(crtcReply);
            }

        }
        else{
            std::cout << "[INFO XCB RANDR]\tOutput is not in use!\n\n";
        }

        free(infoReply);
    }

    free(resReply);

    std::cout << "-" << std::endl;
}

void Bar::Run(){
    while(true){
        xcb_generic_event_t* e = xcb_wait_for_event(m_conn);
        if(!e) break;

        switch(e->response_type & ~0x80){
        case XCB_EXPOSE:
            xcb_image_text_8(m_conn, std::strlen(m_gcDemoText), m_window, m_gc, 10, 20, m_gcDemoText);
            xcb_flush(m_conn);
            break;
        default:
            break;
        }

        free(e);
    }
}

// PRIVATES

bool Bar::ChangeWindowBG(const uint32_t& col){
    xcb_alloc_color_reply_t* colReply;

    const uint16_t r = ((col >> 16) & 0xFF) * 257;
    const uint16_t g = ((col >> 8)  & 0xFF) * 257;
    const uint16_t b = ( col        & 0xFF) * 257;

    xcb_alloc_color_cookie_t colCookie = xcb_alloc_color(m_conn, m_colMap, r, g, b);
    colReply = xcb_alloc_color_reply(m_conn, colCookie, NULL);

    m_valueList[0] = colReply->pixel;
    xcb_change_window_attributes(m_conn, m_window, XCB_CW_BACK_PIXEL, m_valueList);

    free(colReply);

    return true;
}

bool Bar::ChangeWindowSizePos(){
    const uint32_t values[] {m_barX, m_barY, m_barW, m_barH};

    printf("X: %i | Y: %i | W: %i | H: %i\n", m_barX, m_barY, m_barW, m_barH);

    xcb_configure_window(m_conn, m_window, XCB_CONFIG_WINDOW_X
                                         | XCB_CONFIG_WINDOW_Y
                                         | XCB_CONFIG_WINDOW_WIDTH
                                         | XCB_CONFIG_WINDOW_HEIGHT, &values);
    
    return true;
}

const uint32_t Bar::HexStrToUint32(const std::string& str) const {
    return static_cast<uint32_t>(std::stoul(str, nullptr, 16));
}

void Bar::GetMonitorFromStrAndSetX(const std::string& name){
    for(int i = 0; i < m_numDisplays; ++i){
        if(name == m_monitorNames[i]){
            m_barX += m_crtcs[i]->x;
            m_barY += m_crtcs[i]->y;
        }
    }
}

void Bar::LoadConfig(){
    if(m_cfgMgr.Parse()){
        const auto& cfgMap = m_cfgMgr.GetConfigFile();

        std::unordered_map<std::string, std::function<void(const std::string&)>> cfgHandlers = {
            {"bg_color", [this](const std::string& value){m_BG_COL = HexStrToUint32(value); ChangeWindowBG(m_BG_COL);}},
            {"fg_color", [this](const std::string& value){m_FG_COL = HexStrToUint32(value);}},
            {"bar_width", [this](const std::string& value){m_barW = std::stoi(value);}},
            {"bar_height", [this](const std::string& value){m_barH = std::stoi(value);}},
            {"target_monitor", [this](const std::string& value){GetMonitorFromStrAndSetX(value);}},
            {"bar_x", [this](const std::string& value){m_barX += std::stoi(value);}},
            {"bar_y", [this](const std::string& value){m_barY += std::stoi(value);}},
        };

        for(const auto& [key, handler] : cfgHandlers){
            auto iter = cfgMap.find(key);
            if(iter != cfgMap.end()){
                handler(iter->second);
            }
        }
    }

    ChangeWindowSizePos();
}

xcb_atom_t Bar::GetAtom(const char* atomName){
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(m_conn, 0, std::strlen(atomName), atomName);
    xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(m_conn, cookie, nullptr);

    if(!reply) return XCB_NONE;

    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}
