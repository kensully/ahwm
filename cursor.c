/* $Id$
 * This was written by Alex Hioreanu in 2001.
 * This code is in the public domain and the author disclaims all
 * copyright privileges.
 */

#include <X11/Xlib.h>
#include <X11/cursorfont.h>

#include "cursor.h"
#include "xwm.h"

Cursor cursor_normal = None;
Cursor cursor_moving = None;
Cursor cursor_sizing = None;

Cursor cursor_direction_map[9];

int cursor_init()
{
    cursor_normal = XCreateFontCursor(dpy, XC_left_ptr);
    cursor_moving = XCreateFontCursor(dpy, XC_fleur);
    cursor_sizing = XCreateFontCursor(dpy, XC_sizing);

    /* these aren't too pretty, but the ones that I drew to replace
     * these really, really suck */
    cursor_direction_map[0] = XCreateFontCursor(dpy, XC_top_left_corner);
    cursor_direction_map[1] = XCreateFontCursor(dpy, XC_top_right_corner);
    cursor_direction_map[2] = XCreateFontCursor(dpy, XC_bottom_right_corner);
    cursor_direction_map[3] = XCreateFontCursor(dpy, XC_bottom_left_corner);
    cursor_direction_map[4] = XCreateFontCursor(dpy, XC_top_side);
    cursor_direction_map[5] = XCreateFontCursor(dpy, XC_bottom_side);
    cursor_direction_map[6] = XCreateFontCursor(dpy, XC_right_side);
    cursor_direction_map[7] = XCreateFontCursor(dpy, XC_left_side);
    cursor_direction_map[8] = cursor_normal;

    return 1;
}
