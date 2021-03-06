/* $Id$ */
/* Copyright (c) 2001 Alex Hioreanu.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This contains functions to bind actions to mouse clicks and key
 * strokes, and various other utilities for dealing with the mouse and
 * keyboard.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"

#include <X11/Xlib.h>
#include "client.h"

/*
 * Modifier masks which may be arbitrarily mapped by the user
 */

extern unsigned int MetaMask, SuperMask, HyperMask, AltMask, ModeMask;

/*
 * Mask of all 'locking' modifiers
 */

extern unsigned int AllLocksMask;

/*
 * initialize or reinitialize keyboard module's internals (called at
 * startup and whenever keyboard mapping changed)
 * No dependencies
 */

void keyboard_init();

/*
 * Functions which are called in response to mouse and keyboard events
 * all take this form.  See prefs.h for the definition of the arglist
 * type.  Each bindable function must do its own
 * type-checking/type-mangling.
 */

/* avoid header loop with prefs.h */
struct _arglist;

typedef void (*key_fn)(XEvent *, struct _arglist *);
typedef key_fn mouse_fn;

/*
 * An example function of the above type which does nothing
 */

void keyboard_ignore(XEvent *e, struct _arglist *ignored);
extern mouse_fn mouse_ignore;

/*
 * A special function of the above type, put here for lack of a better
 * place.  This will "quote" the next keyboard or mouse event and send
 * it to the client instead of processesing it; this also changes the
 * root background to white to let user know that something is being
 * quoted.  You can cancel a quote by simply clicking on a titlebar or
 * doing any other action which will generate a grab but which the
 * client doesn't care about or can't see.
 */

void keyboard_quote(XEvent *e, struct _arglist *ignored);
extern mouse_fn mouse_quote;

/*
 * When we 'unquote' and event, this function is called.  This will
 * read some parts of the given event and write out some other parts
 * and then try to send the event to the window which would have
 * received it using XSendEvent().  The sent event SHOULD be identical
 * to a real event except that it will have the synthetic flag.  This
 * can be used to replay any arbitrary keyboard event which was
 * grabbed on accident.
 * 
 * The argument is modified.
 * 
 * NB:  my version of xterm has an option allowSendEvents, which by
 * default will make xterm ignore all generated events (including the
 * sythetic event we will send it).  Thus, this will probably not work
 * for most people's xterms.  Specifically, xterm will ignore all
 * synthetic ButtonPress, ButtonRelease, KeyPress and KeyRelease
 * events when the option is on.
 */

void keyboard_replay(XKeyEvent *e);

/*
 * This function parallels keyboard_replay() - it tries to send the
 * given event to a client using XSendEvent().  Works in all cases
 * I've seen but may not have the exact same semantics as a 'real'
 * event.
 */

void mouse_replay(XButtonEvent *e);

/*
 * Bind a key to a function.  KEYCODE and MODIFIERS are the Keycode
 * and Modifiers to bind; DEPRESS is KEYBOARD_DEPRESS or
 * KEYBOARD_RELEASE (one or the other, not a logical OR), indicating
 * whether to call the function on key press or release; FN is the
 * function to be called, with the above semantics.  If duplicate
 * keybindings are assigned, the most recent is used.
 */

void keyboard_bind_ex(unsigned int keycode, unsigned int modifiers,
                      int depress, key_fn fn, struct _arglist *args);

#define KEYBOARD_DEPRESS KeyPress
#define KEYBOARD_RELEASE KeyRelease

/*
 * Set a function to be called in response to a mouse button event,
 * very similar to keyboard_bind_ex().  The "location"
 * argument denotes where we are to listen for the mouse event - NB
 * that it is NOT possible to listen for button events on the actual
 * client window (against ICCCM and raises all sorts of implementation
 * problems).  If you need to listen for an event on the client
 * window, listen for an event on the client frame and set the
 * function for the client titlebar to mouse_ignore.  The "location"
 * parameter may be a logical OR of the locations defined below.
 */

typedef enum { MOUSE_CLICK, MOUSE_DRAG, MOUSE_DOUBLECLICK } click_type;

void mouse_bind_ex(unsigned int button, unsigned int modifiers,
                   click_type type, int location, mouse_fn fn,
                   struct _arglist *args);

#define MOUSE_NOWHERE    00
#define MOUSE_TITLEBAR   01
#define MOUSE_ROOT       02
#define MOUSE_FRAME      04
#define MOUSE_EVERYWHERE (MOUSE_NOWHERE | MOUSE_TITLEBAR | \
                          MOUSE_ROOT | MOUSE_FRAME)

/*
 * This works identically to keyboard_bind_ex except that the
 * keycode and modifiers are parsed from a string.
 * 
 * The grammar for KEYSTRING has tokens which are:
 * 1.  One of the symbols from <X11/keysym.h> with the 'XK_' prefix
 *     removed EXCEPT for the following symbols which are NOT tokens:
 *     Shift_L
 *     Shift_R
 *     Control_L
 *     Control_R
 *     Meta_L
 *     Meta_R
 *     Alt_L
 *     Alt_R
 *     Super_L
 *     Super_R
 *     Hyper_L
 *     Hyper_R
 *     Caps_Lock
 *     Shift_Lock
 *     All of the symbols in this group are case-sensitive.
 * 
 * 2.  One of the following symbols:
 *     Shift, ShiftMask          // standard
 *     Control, ControlMask      // standard
 *     Mod1, Mod1Mask            // standard
 *     Mod2, Mod2Mask            // standard
 *     Mod3, Mod3Mask            // standard
 *     Mod4, Mod4Mask            // standard
 *     Mod5, Mod5Mask            // standard
 *     Alt, AltMask              // nonstandard
 *     Meta, MetaMask            // nonstandard
 *     Hyper, HyperMask          // nonstandard
 *     Super, SuperMask          // nonstandard
 *     All of the symbols in this group are case-insensitive.
 * 
 * The symbols from group (2) above which are marked "nonstandard" are
 * not well-defined; there is a complex relationship between them
 * which is defined by ICCCM and this takes into account.  You can see
 * what keysyms and keycodes your keys generate with 'xev'; you can
 * see what modifier bits they correspond to using 'xmodmap -pm'; you
 * can see the keycode to keysym binding using 'xmodmap -pk'.
 * 
 * The grammar (informally) is as follows:
 * 
 * STRING       ::= MODLIST* WHITESPACE KEY
 * WHITESPACE   ::= ('\t' | ' ' | '\v' | '\f' | '\n' | '\r')*
 * MODLIST      ::= MODIFIER WHITESPACE '|' WHITESPACE
 * MODIFIER     ::= <one of the above symbols from group 2>
 * KEY          ::= <one of the above symbols from group 1>
 * 
 * For example:
 * "Meta | Shift | Control | e" is ok
 * " aLT | Tab   " is ok
 * "Alt | TAB " is NOT ok
 * "Shift | a" should behave the same as "A"
 * "Meta | a" is usually (not always) equivalent to "Mod1 | a"
 * 
 * FIXME:  lowercase alphabetic keys so we don't have to deal with
 * ICCCM crap
 */

void keyboard_bind(char *keystring, int depress,
                   key_fn fn, struct _arglist *args);

/*
 * This works very similar to keyboard_bind except the grammar
 * is a bit different:
 * 
 * STRING       ::= MODLIST* WHITESPACE BUTTON
 * WHITESPACE   ::= ('\t' | ' ' | '\v' | '\f' | '\n' | '\r')*
 * MODLIST      ::= MODIFIER WHITESPACE '|' WHITESPACE
 * MODIFIER     ::= <any MODIFIER as in keyboard_parse_string()>
 * BUTTON       ::= 'Button1'
 *               |  'Button2'
 *               |  'Button3'
 *               |  'Button4'
 *               |  'Button5'
 * 
 * This function treats all tokens as case-insensitive.
 */

void mouse_bind(char *mousestring, click_type type,
                int location, mouse_fn fn, struct _arglist *args);

/*
 * these functions allow one to unbind all key or mouse event that
 * have been bound above
 */
void keyboard_unbind_ex(unsigned int keycode, unsigned int modifiers,
                        int depress);
void mouse_unbind_ex(unsigned int button, unsigned int modifiers,
                     click_type type, int location);
void keyboard_unbind(char *keystring, int depress);
void mouse_unbind(char *mousestring, click_type type, int location);

/*
 * Do a "soft" grab on all the keys that are of interest to us - this
 * should be called once when the window is mapped.  This does not
 * take a client argument as sometimes we need to grab the keys of a
 * window that isn't a client (like when we map an override_redirect
 * window).
 * 
 * If you are grabbing keys of a client, always check
 * client->dont_bind_keys before calling this function.
 */

void keyboard_grab_keys(Window w);

/* Undo everything that keyboard_grab_keys() does */

void keyboard_ungrab_keys(Window w);

/*
 * Grab the mouse buttons we use for a specified window
 * 
 * NB:  while you should call keyboard_grab_keys() on every window
 * (including those with override_redirect), you should NOT grab the
 * mouse buttons of any window with override_redirect.  This does not
 * check for override_redirect.  Shouldn't make a big difference
 * really as most override_redirect windows will grab the keyboard and
 * mouse.
 */

void mouse_grab_buttons(client_t *client);

/* undo mouse_grab_buttons() */

void mouse_ungrab_buttons(client_t *client);

/* 
 * process a key event
 * returns True if handled the event, False if ignored it
 */

Bool keyboard_handle_event(XKeyEvent *xevent);

/*
 * Whenever a mouse event is received it should be passed to this
 * function; this includes XButtonEvent, etc, except for XMotionEvent,
 * which is dealt with as a special case within the event loops of the
 * functions that care about mouse motion.  The pointer will NOT be
 * grabbed when this function returns.
 * returns True if handled the event, False if ignored it
 */

Bool mouse_handle_event(XEvent *e);

/*
 * This will return the address of the function that would have been
 * invoked for the given XKeyEvent.  If 'args' is non-null, it will
 * also set it to the arguments for the function to invoke.
 */

key_fn keyboard_find_function(XKeyEvent *xevent, struct _arglist **args);

/*
 * Returns a mask from a keycode, representing all the modifiers the
 * keycode generates.  Returns zero if keycode does not generate any
 * modifiers.
 */
int keyboard_get_modifier_mask(int keycode);

/*
 * This is kind of ugly - in move-resize.c, move_client() and
 * resize_client() want to know the button which initiated the drag
 * action and the initial coordinates of the drag action.
 */

void mouse_get_drag_info(unsigned int *button, int *x, int *y);

#endif /* KEYBOARD_H */
