#ifndef MIBAR_UTILS_H
#define MIBAR_UTILS_H

#include <cstdlib>
#include <cstring>

#include <xcb/xcb.h>

inline xcb_atom_t GetAtom(xcb_connection_t* conn, const char* atomName){
    xcb_intern_atom_cookie_t c = xcb_intern_atom(conn, 0, strlen(atomName), atomName);
    xcb_intern_atom_reply_t* r = xcb_intern_atom_reply(conn, c, nullptr);

    if(!r) return XCB_NONE;

    xcb_atom_t a = r->atom;
    free(r);
    return a;
}

inline bool TestCookie(xcb_void_cookie_t cookie, xcb_connection_t* conn){
    xcb_generic_error_t* err = xcb_request_check(conn, cookie);
    return err ? true : false;
}

#endif