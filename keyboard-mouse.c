/* $Id$
 * This was written by Alex Hioreanu in 2001.
 * This code is in the public domain and the author disclaims all
 * copyright privileges.
 */

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "keyboard.h"
#include "client.h"
#include "malloc.h"

#ifndef MIN
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

typedef struct _keybinding {
    unsigned int keycode;
    unsigned int modifiers;
    int depress;
    key_fn function;
    void *arg;
    struct _keybinding *next;
} keybinding;

static keybinding *bindings = NULL;

void keyboard_ignore(XEvent *e, void *v)
{
    return;
}

void keyboard_set_function_ex(unsigned int keycode, unsigned int modifiers,
                              int depress, key_fn fn, void *arg)
{
    keybinding *newbinding;

    newbinding = Malloc(sizeof(keybinding));
    if (newbinding == NULL) {
        fprintf(stderr, "XWM: Cannot bind key, out of memory\n");
        return;
    }
    newbinding->next = bindings;
    newbinding->keycode = keycode;
    newbinding->modifiers = modifiers;
    newbinding->depress = depress;
    newbinding->function = fn;
    newbinding->arg = arg;
    bindings = newbinding;
}

/* FIXME: should also apply the bindings to all active clients */

void keyboard_set_function(char *keystring, int depress,
                           key_fn fn, void *arg)
{
    unsigned int keycode;
    unsigned int modifiers;

    if (keyboard_parse_string(keystring, &keycode, &modifiers) != 1) {
        fprintf(stderr, "XWM: Cannot bind key, bad keystring '%s'\n", keystring);
        return;
    }
    keyboard_set_function_ex(keycode, modifiers, depress, fn, arg);
}

void keyboard_grab_keys(client_t *client)
{
    keybinding *kb;

    for (kb = bindings; kb != NULL; kb = kb->next) {
        XGrabKey(dpy, kb->keycode, kb->modifiers, client->frame, True,
                 GrabModeAsync, GrabModeAsync);
    }
}

void keyboard_process(XKeyEvent *xevent)
{
    keybinding *kb;
    int code;

#ifdef DEBUG
    KeySym ks;

    ks = XKeycodeToKeysym(dpy, xevent->keycode, 0);
    printf("\twindow 0x%08X, keycode %d, state %d, keystring %s\n",
           (unsigned int)xevent->window, xevent->keycode,
           xevent->state, XKeysymToString(ks));
#endif /* DEBUG */

    code = xevent->keycode;

    for (kb = bindings; kb != NULL; kb = kb->next) {
        if (kb->keycode == code) {
            if (kb->modifiers == xevent->state
                && (kb->depress & xevent->type) == xevent->type) {
                (*kb->function)((XEvent *)xevent, kb->arg);
                return;
            }
        }
    }
}

/*
 * This is simple enough that we don't need to bring in lex (or, God
 * forbid, yacc).  Looks ugly, mostly just string manipulation.
 */
int keyboard_parse_string(char *keystring, unsigned int *keycode_ret,
                          unsigned int *modifiers_ret)
{
    char buf[512];
    char *cp1, *cp2;
    unsigned int keycode;
    unsigned int modifiers, tmp_modifier;
    KeySym ks;

    if (keystring == NULL) return 0;
    memset(buf, 0, 512);
    modifiers = 0;

    while (*keystring != '\0') {
        while (isspace(*keystring)) keystring++;
        cp1 = strchr(keystring, '|');
        if (cp1 == NULL) {
            strncpy(buf, keystring, 512);
            buf[511] = '\0';
            while (isspace(*(buf + strlen(buf) - 1)))
                *(buf + strlen(buf) - 1) = '\0';
            ks = XStringToKeysym(buf);
            if (ks == NoSymbol) {
                fprintf(stderr, "XWM: Couldn't figure out '%s'\n", buf);
                return 0;
            }
            keycode = XKeysymToKeycode(dpy, ks);
            if (keycode == 0) {
                fprintf(stderr,
                        "XWM: XKeysymToKeycode failed somehow "
                        "(perhaps unmapped?)");
                return 0;
            }
            *modifiers_ret = modifiers;
            *keycode_ret = keycode;
            return 1;
        }
        /* found a modifier key */
        cp2 = cp1 - 1;
        while (isspace(*cp2)) cp2--;
        memcpy(buf, keystring, MIN(511, cp2 - keystring + 1));
        buf[MIN(511, cp2 - keystring + 1)] = '\0';

        /* FIXME: strcasecmp may not be very standard, rewrite */
        if (strcasecmp(buf, "Mod1") == 0) {
            tmp_modifier = Mod1Mask;
        } else if (strcasecmp(buf, "Mod1Mask") == 0) {
            tmp_modifier = Mod1Mask;
        } else if (strcasecmp(buf, "Mod2") == 0) {
            tmp_modifier = Mod2Mask;
        } else if (strcasecmp(buf, "Mod2Mask") == 0) {
            tmp_modifier = Mod2Mask;
        } else if (strcasecmp(buf, "Mod3") == 0) {
            tmp_modifier = Mod3Mask;
        } else if (strcasecmp(buf, "Mod3Mask") == 0) {
            tmp_modifier = Mod3Mask;
        } else if (strcasecmp(buf, "Mod4") == 0) {
            tmp_modifier = Mod4Mask;
        } else if (strcasecmp(buf, "Mod4Mask") == 0) {
            tmp_modifier = Mod4Mask;
        } else if (strcasecmp(buf, "Mod5") == 0) {
            tmp_modifier = Mod5Mask;
        } else if (strcasecmp(buf, "Mod5Mask") == 0) {
            tmp_modifier = Mod5Mask;
        } else if (strcasecmp(buf, "Shift") == 0) {
            tmp_modifier = ShiftMask;
        } else if (strcasecmp(buf, "ShiftMask") == 0) {
            tmp_modifier = ShiftMask;
        } else if (strcasecmp(buf, "Control") == 0) {
            tmp_modifier = ControlMask;
        } else if (strcasecmp(buf, "ControlMask") == 0) {
            tmp_modifier = ControlMask;
        } else if (strcasecmp(buf, "Meta") == 0) {
            tmp_modifier = Mod1Mask;
        } else if (strcasecmp(buf, "MetaMask") == 0) {
            tmp_modifier = Mod1Mask;
        } else if (strcasecmp(buf, "Super") == 0) {
            tmp_modifier = Mod3Mask;
        } else if (strcasecmp(buf, "SuperMask") == 0) {
            tmp_modifier = Mod3Mask;
        } else if (strcasecmp(buf, "Hyper") == 0) {
            tmp_modifier = Mod4Mask;
        } else if (strcasecmp(buf, "HyperMask") == 0) {
            tmp_modifier = Mod4Mask;
        } else if (strcasecmp(buf, "Alt") == 0) {
            tmp_modifier = Mod1Mask;
        } else if (strcasecmp(buf, "AltMask") == 0) {
            tmp_modifier = Mod1Mask;
        } else {
            fprintf(stderr, "XWM: Could not figure out modifier '%s'\n", buf);
            return 0;
        }
        modifiers |= tmp_modifier;

        keystring = cp1 + 1;
    }
    return 0;
}
