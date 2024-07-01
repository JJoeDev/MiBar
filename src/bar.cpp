#include <cstring>
#include <iostream>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

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
    xcb_screen_t* screen = iter.data;

    m_colMap = screen->default_colormap;

    m_window = xcb_generate_id(m_conn);

    m_valueMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    m_valueList[0] = screen->black_pixel;
    m_valueList[1] = XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(m_conn,
                      XCB_COPY_FROM_PARENT,
                      m_window,
                      screen->root,
                      m_barX,
                      m_barY,
                      m_barW,
                      m_barH,
                      0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual,
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
    xcb_disconnect(m_conn);
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

    xcb_alloc_color_cookie_t colCookie = xcb_alloc_color(m_conn, m_colMap, (col >> 16) & 0xFFFF, (col >> 8) & 0xFFFF, col & 0xFFFF);
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

uint32_t Bar::HexStrToUint32(const std::string& str){
    std::cout << "STRING: " << str << '\n';
    return static_cast<uint32_t>(std::stoul(str, nullptr, 16));
}

void Bar::LoadConfig(){
    if(m_cfgMgr.Parse()){
        auto cfgMap = m_cfgMgr.GetConfigFile();

        if(cfgMap.find("bg_color") != cfgMap.end()){
            m_BG_COL = HexStrToUint32(cfgMap["bg_color"]);
            //m_gcVal[1] = m_BG_COL;
            //changeWindowBG(m_BG_COL);
        }
        if(cfgMap.find("fg_color") != cfgMap.end()){
            m_FG_COL = HexStrToUint32(cfgMap["fg_color"]);
        }
        if(cfgMap.find("bar_width") != cfgMap.end()){
            m_barW = std::stoi(cfgMap["bar_width"]);
        }
        if(cfgMap.find("bar_height") != cfgMap.end()){
            m_barH = std::stoi(cfgMap["bar_height"]);
        }
        if(cfgMap.find("bar_x") != cfgMap.end()){
            m_barX = std::stoi(cfgMap["bar_x"]);
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
