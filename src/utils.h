#ifndef MIBAR_UTILS_H
#define MIBAR_UTILS_H

#include <cstdlib>
#include <cstring>

#include <xcb/xcb.h>

/**
    A function that will try to get the requested atom

    @param conn The xcb connection to the X server
    @param atomName The name of the atom you are trying to fetch

    @result Returns an xcb_atom_t copy
*/
inline xcb_atom_t GetAtom(xcb_connection_t* conn, const char* atomName){
    xcb_intern_atom_cookie_t c = xcb_intern_atom(conn, 0, strlen(atomName), atomName);
    xcb_intern_atom_reply_t* r = xcb_intern_atom_reply(conn, c, nullptr);

    if(!r) return XCB_NONE;

    xcb_atom_t a = r->atom;
    free(r);
    return a;
}

/**
    A function that will test to see if the provided cookie is valid

    @param cookie Takes any xcb_*_cookie_t
    @param conn The xcb connection to the X server

    @result A boolean value that is true if no error was chaught and false if an error was chaught
*/
inline bool TestCookie(xcb_void_cookie_t cookie, xcb_connection_t* conn){
    xcb_generic_error_t* err = xcb_request_check(conn, cookie);
    return err ? true : false;
}

#endif