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
#include "workspace.h"
#include "debug.h"
#include "event.h"
#include "focus.h"

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

Bool keyboard_is_lock[8] = { False };

unsigned int MetaMask, SuperMask, HyperMask, AltMask, ModeMask;

unsigned int AllLocksMask;

static keybinding *bindings = NULL;
static unsigned int *modifier_combinations = NULL;

static void warn(KeySym new, char *old, char *mod)
{
    fprintf(stderr,
            "XWM WARNING:  '%s' is generated by both the keysyms\n"
            "'%s' and '%s' on different modifiers.\n"
            "This is almost certainly an error, and some applications\n"
            "applications may misbehave.  Using the last modifier mapping.\n"
            "The only way to remove this error message is to remap\n"
            "your modifiers using 'xmodmap'\n",
            mod, old, XKeysymToString(new));
}

static void figure_lock(KeySym keysym, int bit)
{
    int i;
    static KeySym locks[] = {
        XK_Scroll_Lock,
        XK_Num_Lock,
        XK_Caps_Lock,
        XK_Shift_Lock,
        XK_Kana_Lock,
        XK_ISO_Lock,
        XK_ISO_Level3_Lock,
        XK_ISO_Group_Lock,
        XK_ISO_Next_Group_Lock,
        XK_ISO_Prev_Group_Lock,
        XK_ISO_First_Group_Lock,
        XK_ISO_Last_Group_Lock,
    };

    for (i = 0; i < 12; i++) {
        if (keysym == locks[i]) {
            keyboard_is_lock[bit] = True;
        }
    }
}

void keyboard_init()
{
    XModifierKeymap *xmkm;
    int i, j, k;
    int meta_bit, super_bit, hyper_bit, alt_bit, mode_bit;
    char *meta_key, *super_key, *hyper_key, *alt_key, *mode_key;
    KeyCode keycode;
    KeySym keysym;

    meta_bit = super_bit = hyper_bit = alt_bit = mode_bit = -1;
    xmkm = XGetModifierMapping(dpy);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < xmkm->max_keypermod; j++) {
            keycode = xmkm->modifiermap[i * xmkm->max_keypermod + j];
            if (keycode != 0) {
                for (k = 0; k < 4; k++) {
                    keysym = XKeycodeToKeysym(dpy, keycode, k);
                    figure_lock(keysym, i);
                    switch (keysym) {
                        case XK_Meta_L:
                        case XK_Meta_R:
                            if (meta_bit != -1 && meta_bit != i)
                                warn(keysym, meta_key, "Meta");
                            meta_bit = i;
                            meta_key = XKeysymToString(keysym);
                            break;
                            
                        case XK_Alt_L:
                        case XK_Alt_R:
                            if (alt_bit != -1 && alt_bit != i)
                                warn(keysym, alt_key, "Alt");
                            alt_bit = i;
                            alt_key = XKeysymToString(keysym);
                            break;

                        case XK_Super_L:
                        case XK_Super_R:
                            if (super_bit != -1 && super_bit != i)
                                warn(keysym, super_key, "Super");
                            super_bit = i;
                            super_key = XKeysymToString(keysym);
                            break;

                        case XK_Hyper_L:
                        case XK_Hyper_R:
                            if (hyper_bit != -1 && hyper_bit != i)
                                warn(keysym, hyper_key, "Hyper");
                            hyper_bit = i;
                            hyper_key = XKeysymToString(keysym);
                            break;
                            
                        case XK_Mode_switch:
                            if (mode_bit != -1 && mode_bit != i)
                                warn(keysym, mode_key, "Mode Switch");
                            mode_bit = i;
                            mode_key = XKeysymToString(keysym);
                            break;
                    }
                }
            }
        }
    }
    
    XFreeModifiermap(xmkm);
                
    MetaMask  = 0;
    SuperMask = 0;
    HyperMask = 0;
    AltMask   = 0;
    ModeMask  = 0;
    if (meta_bit  != -1) MetaMask  = (1 << meta_bit);
    if (super_bit != -1) SuperMask = (1 << super_bit);
    if (hyper_bit != -1) HyperMask = (1 << hyper_bit);
    if (alt_bit   != -1) AltMask   = (1 << alt_bit);
    if (mode_bit  != -1) ModeMask  = (1 << mode_bit);

    AllLocksMask = 0;
    j = 0;
    for (i = 0; i < 8; i++) {
        if (keyboard_is_lock[i]) {
            AllLocksMask |= (1 << i);
            j *= j + 1;
            if (j == 0) j = 1;
        }
    }
    if (modifier_combinations != NULL) Free(modifier_combinations);
    modifier_combinations = Malloc(sizeof(unsigned int) * j);
    /* check for malloc returning NULL is in keyboard_modifier_combinations */
}

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

static void modifier_combinations_helper(unsigned int state,
                                         int *n, int bit)
{
    int i;

    if (!keyboard_is_lock[bit]) return;

    state |= (1 << bit);
    modifier_combinations[(*n)++] = state;
    for (i = bit + 1; i < 8; i++) {
        modifier_combinations_helper(state, n, i);
    }
}

unsigned int *keyboard_modifier_combinations(unsigned int mods, int *n)
{
    int i;
    
    *n = 0;
    if (modifier_combinations == NULL) return NULL;
    for (i = 0; i < 8; i++) {
        modifier_combinations_helper(mods, n, i);
    }
    return modifier_combinations;
}

/*
 * FIXME:  figure out if this should even be here, not used right now
 */

KeySym keyboard_event_to_keysym(XKeyEvent *xevent)
{
    int index;

    if (ModeMask != 0 && xevent->state & ModeMask)
        index = 2;
    else
        index = 0;

/*
 * Xlib docs say that this function is supposed to work like this from
 * here on out:
 * 
 * if (numlock is on AND second keysym is a keypad keysym (XK_KP_)):
 *     if (shift is on OR (lock is on AND lock is shiftlock):
 *         return first keysym
 *     else:
 *         return second keysym
 * else if (shift is off AND lock is off):
 *     return first keysym
 * else if (shift is off AND lock is on AND lock is capslock):
 *     if (first keysym is lowercase alphabetic):
 *         return uppercase of first keysym
 *     else:
 *         return first keysym
 * else if (shift is on AND lock is on AND lock is capslock):
 *     if (second keysym is lowercase alphabetic):
 *         return lowercase of second keysym
 *     else:
 *         return second keysym
 * else if (shift is on OR (lock is on and lock is shiftlock)):
 *     return second keysym
 * 
 * This is fine and good for applications that want to change the key
 * event into some displayable text, but is not well-suited for our
 * purposes, which is to bind a function to a distinguishable key
 * event.  We will ignore all the various locks and shifts, and only
 * worry about the mode switch key.
 */
    
    return XKeycodeToKeysym(dpy, xevent->keycode, index);
}

void keyboard_grab_keys(client_t *client)
{
    keybinding *kb;
    unsigned int *combs;
    int i, n;

    for (kb = bindings; kb != NULL; kb = kb->next) {
        XGrabKey(dpy, kb->keycode, kb->modifiers, client->frame, True,
                 GrabModeAsync, GrabModeAsync);
        combs = keyboard_modifier_combinations(kb->modifiers, &n);
        for (i = 0; i < n; i++) {
            XGrabKey(dpy, kb->keycode, combs[i], client->frame, True,
                     GrabModeAsync, GrabModeAsync);
        }
    }
}

Bool quoting = False;
static Time last_quote_time;

void keyboard_quote(XEvent *e, void *v)
{
    XSetWindowAttributes xswa;

    debug(("\tQuoting\n"));
    quoting = True;
    xswa.background_pixel = white;
    XChangeWindowAttributes(dpy, root_window, CWBackPixel, &xswa);
    XClearWindow(dpy, root_window);
    last_quote_time = e->xkey.time;
}

/* FIXME:  use XGetInputFocus */
void keyboard_unquote(XKeyEvent *e)
{
    XSetWindowAttributes xswa;
    
    debug(("\tUnquoting\n"));
    quoting = False;
    xswa.background_pixel = workspace_pixels[workspace_current - 1];
    XChangeWindowAttributes(dpy, root_window, CWBackPixel, &xswa);
    XClearWindow(dpy, root_window);
    e->time = last_quote_time;
    e->window = focus_current->window;
    XSendEvent(dpy, focus_current->window, True,
               NoEventMask, (XEvent *)e);
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
    
    if (quoting) {
        keyboard_unquote(xevent);
        return;
    }

    for (kb = bindings; kb != NULL; kb = kb->next) {
        if (kb->keycode == code) {
            if (kb->modifiers == (xevent->state & (~AllLocksMask))
                && (kb->depress == xevent->type)) {
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
            tmp_modifier = MetaMask;
        } else if (strcasecmp(buf, "MetaMask") == 0) {
            tmp_modifier = MetaMask;
        } else if (strcasecmp(buf, "Super") == 0) {
            tmp_modifier = SuperMask;
        } else if (strcasecmp(buf, "SuperMask") == 0) {
            tmp_modifier = SuperMask;
        } else if (strcasecmp(buf, "Hyper") == 0) {
            tmp_modifier = HyperMask;
        } else if (strcasecmp(buf, "HyperMask") == 0) {
            tmp_modifier = HyperMask;
        } else if (strcasecmp(buf, "Alt") == 0) {
            tmp_modifier = AltMask;
        } else if (strcasecmp(buf, "AltMask") == 0) {
            tmp_modifier = AltMask;
        } else {
            fprintf(stderr, "XWM: Could not figure out modifier '%s'\n", buf);
            return 0;
        }
        modifiers |= tmp_modifier;

        keystring = cp1 + 1;
    }
    return 0;
}
