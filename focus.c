/* $Id$
 * This was written by Alex Hioreanu in 2001.
 * This code is in the public domain and the author disclaims all
 * copyright privileges.
 */

/*
 * OK, the stuff in here gets a little bit confusing because we have
 * to do things *just so* or client applications will misbehave.  Each
 * function has a distinct purpose.  There are a number of separate
 * things we can do in any given function:
 * 
 * 1.  update some pointers, like focus_current or focus_stacks[]
 * 2.  update the focus stack (focus order)
 * 3.  update clients' titlebars to show input focus
 * 4.  call XSetInputFocus()
 * 5.  raise the active client
 */

/* 
 * invariant:
 * 
 * focus_current == focus_stacks[workspace_current - 1]
 * 
 * This should hold whenever we enter or leave the window manager.  In
 * a few fringe cases, focus_current is not the real input focus (ie,
 * what XGetInputFocus() would return), but represents what what the
 * real input focus will be once some condition has been satisfied.
 */

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include "focus.h"
#include "client.h"
#include "workspace.h"
#include "debug.h"
#include "event.h"
#include "ewmh.h"
#include "keyboard-mouse.h"
#include "stacking.h"
#include "malloc.h"

typedef struct _focus_node {
    struct _focus_node *next;
    struct _focus_node *prev;
    client_t *client;
} focus_node;

client_t *focus_current = NULL;

static focus_node *focus_stacks[NO_WORKSPACES] = { NULL };

static XContext focus_contexts[NO_WORKSPACES];

static focus_node *find_node(client_t *);
static void focus_change_current(client_t *, Time, Bool);
static void focus_set_internal(focus_node *, Time, Bool);
static void permute(focus_node *, focus_node *);
static focus_node *get_prev(focus_node *);
static focus_node *get_next(focus_node *);
static void focus_add_internal(focus_node *, int ws, Time timestamp);
static void focus_remove_internal(focus_node *, int ws, Time timestamp);

static Bool in_alt_tab = False; /* see focus_alt_tab, focus_ensure */

void focus_init()
{
    int i;

    for (i = 0; i < NO_WORKSPACES; i++)
        focus_contexts[i] = XUniqueContext();
}

static focus_node *find_node(client_t *client)
{
    focus_node *node;

    if (XFindContext(dpy, client->window,
                     focus_contexts[client->workspace - 1],
                     (void *)&node) != 0) {
        return NULL;
    }
    return node;
}

Bool focus_forall(forall_fn fn, void *v)
{
    focus_node *node, *tmp;

    tmp = node = focus_stacks[workspace_current - 1];
    if (node == NULL) return True;
    do {
        if (fn(node->client, v) == False) return False;
        node = node->next;
    } while (node != tmp);
    return True;
}

/*
 * This will:
 * update pointers
 * update the focus stack
 * update the clients' titlebars 
 * call XSetInputFocus
 */

void focus_add(client_t *client, Time timestamp)
{
    int i;
    focus_node *node;

    if (client->prefs.omnipresent) {
        node = Malloc(NO_WORKSPACES * sizeof(focus_node));
        if (node == NULL) {
            fprintf(stderr, "XWM: out of memory while focusing client\n");
            return;
        }
        for (i = 0; i < NO_WORKSPACES; i++) {
            node[i].client = client;
            focus_add_internal(&node[i], i + 1, timestamp);
        }
    } else {
        node = Malloc(sizeof(focus_node));
        node->client = client;
        focus_add_internal(node, client->workspace, timestamp);
    }
}

static void focus_add_internal(focus_node *node, int ws, Time timestamp)
{
    focus_node *old;

    focus_remove_internal(node, ws, timestamp);
    old = focus_stacks[ws - 1];
    if (old == NULL) {
        debug(("\tsetting focus stack of workspace %d to 0x%08X ('%.10s')\n",
               ws, node->client->window, node->client->name));
        node->next = node;
        node->prev = node;
        focus_stacks[ws - 1] = node;
    } else {
        node->next = old;
        node->prev = old->prev;
        old->prev->next = node;
        old->prev = node;
        focus_stacks[ws - 1] = node;
    }
    if (ws == workspace_current) {
        focus_change_current(node->client, timestamp, True);
    }
    if (XSaveContext(dpy, node->client->window,
                     focus_contexts[ws - 1], (void *)node) != 0) {
        fprintf(stderr, "XWM: XSaveContext failed, could not save window\n");
    }
}

/*
 * This will:
 * update pointers
 * update the focus stack
 * update the clients' titlebars 
 * call XSetInputFocus
 */

void focus_remove(client_t *client, Time timestamp)
{
    int i;
    focus_node *node;

    node = find_node(client);
    if (node == NULL) return;
    if (client->prefs.omnipresent) {
        for (i = 0; i < NO_WORKSPACES; i++) {
            focus_remove_internal(&node[i], i, timestamp);
        }
    } else {
        focus_remove_internal(node, client->workspace - 1, timestamp);
    }
    free(node);
}

/* FIXME: unhash */
static void focus_remove_internal(focus_node *node, int ws, Time timestamp)
{
    focus_node *stack, *orig;

    stack = orig = focus_stacks[ws - 1];
    if (stack == NULL) return;
    do {
        if (stack == node) {
            /* remove from list */
            node->prev->next = node->next;
            node->next->prev = node->prev;
            /* if was focused for workspace, update workspace pointer */
            if (focus_stacks[ws - 1] == node) {
                debug(("\tSetting focus stack of workspace "
                       "%d to 0x%08X ('%.10s')\n",
                       ws, get_next(node)->client->window,
                       get_next(node)->client->name));
                focus_stacks[ws - 1] = get_next(node);
            }
            /* if only client left on workspace, set to NULL */
            if (node->next == node) {
                debug(("\tSetting focus stack of workspace %d to null\n",
                       ws));
                focus_stacks[ws - 1] = NULL;
                node->next = NULL;
                node->prev = NULL;
            }
            /* if removed was focused window, refocus now */
            if (node->client == focus_current) {
                focus_change_current(focus_stacks[ws - 1]->client,
                                     timestamp, True);
            }
            XDeleteContext(dpy, node->client->window,
                           focus_contexts[ws - 1]);
            return;
        }
        stack = stack->next;
    } while (stack != orig);
}

/*
 * This will:
 * update pointers
 * update the focus stack
 * update the clients' titlebars 
 * call XSetInputFocus iff (call_focus_ensure == True)
 */

static void focus_set_internal(focus_node *node, Time timestamp,
                               Bool call_focus_ensure)
{
    focus_node *p;

    if (node != NULL && node->client == focus_current) return;

    if (node == NULL) {
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
        return;
    }
    
    p = focus_stacks[node->client->workspace - 1];
    if (p == NULL) {
        fprintf(stderr, "XWM: current focus list is empty, shouldn't be\n");
        return;
    }
    do {
        if (p == node) {
            debug(("\tSetting focus stack of workspace %d "
                   "to 0x%08X ('%.10s')\n",
                   node->client->workspace, node->client->window,
                   node->client->name));
            focus_stacks[node->client->workspace - 1] = node;
            if (node->client->workspace == workspace_current) {
                focus_change_current(node->client, timestamp,
                                     call_focus_ensure);
            }
            return;
        }
        p = p->next;
    } while (p != focus_stacks[node->client->workspace - 1]);
    fprintf(stderr, "XWM: client not found on focus list, shouldn't happen\n");
}

/*
 * This will:
 * update pointers
 * update the focus stack
 * update the clients' titlebars 
 * call XSetInputFocus
 */

void focus_set(client_t *client, Time timestamp)
{
    focus_node *node, *old;

    node = find_node(client);
    old = find_node(focus_current);
    focus_set_internal(node, timestamp, True);
    if (old != NULL && node != NULL)
        permute(old, node);
}

/*
 * This will:
 * call XSetInputFocus
 * raise active client
 * 
 * This is the only function in the module that will call
 * XSetInputFocus; all other functions call this function.
 */

void focus_ensure(Time timestamp)
{
    if (focus_current == NULL) {
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
        return;
    }

    debug(("\tCalling XSetInputFocus(0x%08X) ('%.10s')\n",
           (unsigned int)focus_current->window, focus_current->name));

    ewmh_active_window_update();

    if (in_alt_tab) {
        stacking_raise(focus_current);
        return;
    }
    
    /* see ICCCM 4.1.7 */
    if (focus_current->xwmh != NULL &&
        focus_current->xwmh->flags & InputHint &&
        focus_current->xwmh->input == False) {
        /* FIXME:  we shouldn't call XSetInputFocus here */
        debug(("\tInput hint is False\n"));
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
    } else {
        debug(("\tInput hint is True\n"));
        XSetInputFocus(dpy, focus_current->window,
                       RevertToPointerRoot, timestamp);
    }
    if (focus_current->protocols & PROTO_TAKE_FOCUS) {
        debug(("\tUses TAKE_FOCUS protocol\n"));
        client_sendmessage(focus_current, WM_TAKE_FOCUS,
                           timestamp, 0, 0, 0);
    } else {
        debug(("\tDoesn't use TAKE_FOCUS protocol\n"));
    }

    stacking_raise(focus_current);
}

/*
 * This will:
 * update pointers
 * update titlebars
 * call XSetInputFocus iff (call_focus_ensure == True)
 */

static void focus_change_current(client_t *new, Time timestamp,
                                 Bool call_focus_ensure)
{
    client_t *old;

    old = focus_current;
    focus_current = new;
    client_paint_titlebar(old);
    client_paint_titlebar(new);
    if (old != new) {
        if (old != NULL && old->focus_policy == ClickToFocus) {
            debug(("\tGrabbing Button 1 of 0x%08x\n", old));
            XGrabButton(dpy, Button1, 0, old->frame,
                        True, ButtonPressMask, GrabModeSync,
                        GrabModeAsync, None, None);
            keyboard_grab_keys(old);
        }
        if (new != NULL && new->focus_policy == ClickToFocus) {
            XUngrabButton(dpy, Button1, 0, new->frame);
        }
    }
    if (call_focus_ensure) focus_ensure(timestamp);
    XFlush(dpy);
}

#ifdef DEBUG
void dump_focus_list()
{
    client_t *client, *orig;

    orig = client = focus_current;

    printf("STACK: ");
    do {
        if (client == NULL) break;
        printf("\t'%s'\n", client->name);
        client = client->next_focus;
    } while (client != orig);
    printf("\n");
}
#else
#define dump_focus_list() /* */
#endif

/*
 * After extensive experimentation, I determined that this is the best
 * way for this function to behave.
 * 
 * We want clients to receive a Focus{In,Out} event when the focus is
 * transferred, and will only transfer the focus when the alt-tab
 * action is completed (when user lets go of alt).  Some clients
 * (notably all applications which use the QT toolkit) will only think
 * that they've received the input focus when they receive a FocusIn,
 * NotfyNormal event.  The problem is that if we transfer the input
 * focus while we have the keyboard grabbed, this will generate
 * FocusIn, NotfyGrab events, which some applications completely
 * ignore (whether or not this is the correct behaviour, I don't care,
 * that's how it is), and then any XSetInputFocus call to the current
 * focus window made after ungrabbing the keyboard will be a no-op;
 * thus QT applications will not believe they have the input focus
 * (and the way QT is set up, QT applications won't take keyboard
 * input in this state).  Needless to say, this is quite annoying.
 * The solution is to delay all calls to XSetInputFocus until we've
 * ungrabbed the keyboard.
 * 
 * This will behave correctly if clients are added or removed while
 * this function is active.  That's the purpose of the in_alt_tab
 * boolean - it ensures we don't call XSetInputFocus while this
 * function is active.  This will return immediately if all the
 * clients disappear from under our nose.
 * 
 * This is still sub-optimal as some clients may receive FocusIn,
 * NotifyPointer events while we are in this function and will
 * incorrectly think they have the input focus and update themselves
 * accordingly.  Xterm does this.  This only lasts until the function
 * exits, but is a bit annoying.  There is no way to fix this other
 * than fixing the bug in xterm (other window managers have the same
 * behaviour).
 */

void focus_alt_tab(XEvent *xevent, void *v)
{
    focus_node *orig_focus;
    focus_node *node;
    unsigned int action_keycode;
    KeyCode keycode_Alt_L, keycode_Alt_R;
    enum { CONTINUE, DONE, REPLAY_KEYBOARD, QUIT } state;

    if (focus_current == NULL) {
        debug(("\tNo clients in alt-tab, returning\n"));
        return;
    }
    debug(("\tEntering alt-tab\n"));
    in_alt_tab = True;
    orig_focus = find_node(focus_current);
    debug(("\torig_focus = '%s'\n",
           orig_focus ? orig_focus->client->name : "NULL"));
    dump_focus_list();
    action_keycode = xevent->xkey.keycode;
    keycode_Alt_L = XKeysymToKeycode(dpy, XK_Alt_L);
    keycode_Alt_R = XKeysymToKeycode(dpy, XK_Alt_R);

    XGrabKeyboard(dpy, root_window, True, GrabModeAsync,
                  GrabModeAsync, CurrentTime);
    state = CONTINUE;
    while (state == CONTINUE) {
        switch (xevent->type) {
            case KeyPress:
                if (xevent->xkey.keycode == action_keycode) {
                    node = find_node(focus_current);
                    if (xevent->xkey.state & ShiftMask) {
                        node = get_prev(node);
                    } else {
                        node = get_next(node);
                    }
                    focus_set_internal(node, event_timestamp, False);
                } else {
                    state = REPLAY_KEYBOARD;
                }
                break;
            case KeyRelease:
                if (xevent->xkey.keycode == keycode_Alt_L
                    || xevent->xkey.keycode == keycode_Alt_R) {
                    /* user let go of all modifiers */
                    state = DONE;
                }
                break;
            default:
                event_dispatch(xevent);
                if (focus_current == NULL) {
                    debug(("\tAll clients gone, returning from alt-tab\n"));
                    state = QUIT;
                }
        }
        if (state == CONTINUE) event_get(ConnectionNumber(dpy), xevent);
    }

    node = find_node(focus_current);
    if (state != QUIT && node != NULL && orig_focus != NULL) {
        permute(orig_focus, node);
    }
    
    XUngrabKeyboard(dpy, CurrentTime);
    /* we want to make sure server knows we've
     * ungrabbed keyboard before calling
     * XSetInputFocus: */
    XSync(dpy, False);
    in_alt_tab = False;
    focus_ensure(CurrentTime);
                    
    if (state == REPLAY_KEYBOARD) {
        if (!keyboard_handle_event(&xevent->xkey))
            keyboard_replay(&xevent->xkey);
    }
    
    debug(("\tfocus_current = '%.10s'\n",
           focus_current ? focus_current->name : "NULL"));
    dump_focus_list();
    debug(("\tLeaving alt-tab\n"));
}

static focus_node *get_prev(focus_node *node)
{
    focus_node *p;

    for (p = node->prev; p != node; p = p->prev) {
        if (!p->client->prefs.skip_alt_tab) return p;
    }
    return node->prev;
}

static focus_node *get_next(focus_node *node)
{
    focus_node *p;

    for (p = node->next; p != node; p = p->next) {
        if (!p->client->prefs.skip_alt_tab) return p;
    }
    return node->next;
}

/*
 * This will swap two elements in the focus stack, moving one to the
 * top of the stack.
 * 
 * ring looks like this:
 * A-B-...-C-D-E-...-F-A
 * and we want it to look like this:
 * D-A-B-...-C-E-...-F-D
 * 
 * special cases:
 * A-A-...  ->  A-A-...  (no change)
 * A-B-A-...  ->  B-A-B-... (no change, just update focus_current)
 * A-D-E-...-F-A  ->  D-A-E-...-F-D  (just swap A & D)
 * A-B-...-C-D-A  ->  D-A-B-...-C-D  (no change, just update focus_current)
 * We actually don't change B in any way, so it's left out.
 */

static void permute(focus_node *A, focus_node *D)
{
    focus_node *C, *E, *F;

    /* if have only one or two elements, or not moving, done */
    if (A == D || (A->next == D && D->next == A))
        return;

    F = A->prev;
    C = D->prev;
    E = D->next;

    if (A->next == D) {
        /* no B or C, just swap A & D */
        A->prev = D;
        D->next = A;
        
        A->next = E;
        E->prev = A;
        
        F->next = D;
        D->prev = F;
        
    } else if (A->prev == D) {
        /* no E or F, no need for change */
    } else {
        /* B and D are separated by at least one node on each side */
        C->next = E;
        E->prev = C;

        F->next = D;
        D->prev = F;

        D->next = A;
        A->prev = D;
    }
}
