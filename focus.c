/* $Id$
 * This was written by Alex Hioreanu in 2001.
 * This code is in the public domain and the author disclaims all
 * copyright privileges.
 */

#include <stdio.h>
#include "focus.h"
#include "client.h"
#include "workspace.h"

client_t *focus_current = NULL;

client_t *focus_stacks[NO_WORKSPACES] = { NULL };

/* 
 * invariant:
 * 
 * focus_current == focus_stacks[workspace_current - 1]
 * 
 * This should hold whenever we enter or leave the window manager.
 * Call focus_ensure() whenever leaving the window manager (and the
 * invariant may not hold) to update 'focus_current'.
 */

void focus_add(client_t *client, Time timestamp)
{
    client_t *old;

    focus_remove(client, timestamp);
    old = focus_stacks[client->workspace - 1];
    if (old == NULL) {
#ifdef DEBUG
        printf("\tsetting focus stack of workspace %d to %s\n",
               client->workspace, client->name);
#endif /* DEBUG */
        client->next_focus = client;
        client->prev_focus = client;
        focus_stacks[client->workspace - 1] = client;
    } else {
        client->next_focus = old;
        client->prev_focus = old->prev_focus;
        old->prev_focus->next_focus = client;
        old->prev_focus = client;
    }
    if (client->workspace == workspace_current) {
        focus_current = client;
        focus_ensure(timestamp);
    }
}

void focus_remove(client_t *client, Time timestamp)
{
    client_t *stack, *orig;

    stack = orig = focus_stacks[client->workspace - 1];
    if (stack == NULL) return;
    do {
        if (stack == client) {
            /* remove from list */
            client->prev_focus->next_focus = client->next_focus;
            client->next_focus->prev_focus = client->prev_focus;
            /* if was focused for workspace, update workspace pointer */
            if (focus_stacks[client->workspace - 1] == client) {
#ifdef DEBUG
                printf("\tsetting focus stack of workspace %d to %s\n",
                       client->workspace, client->prev_focus->name);
#endif /* DEBUG */
                focus_stacks[client->workspace - 1] = client->prev_focus;
            }
            /* if only client left on workspace, set to NULL */
            if (client->next_focus == client) {
#ifdef DEBUG
                printf("setting focus stack of workspace %d to null\n",
                       client->workspace);
#endif /* DEBUG */
                focus_stacks[client->workspace - 1] = NULL;
                client->next_focus = NULL;
                client->prev_focus = NULL;
            }
            /* if removed was focused window, refocus now */
            if (client == focus_current) {
                focus_current = client->prev_focus;
                focus_ensure(timestamp);
            }
            return;
        }
        stack = stack->next_focus;
    } while (stack != orig);
}

void focus_set(client_t *client, Time timestamp)
{
    client_t *p;

    if (client == focus_current) return;

    if (client == NULL) {
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
        return;
    }
    
    p = focus_stacks[client->workspace - 1];
    if (p == NULL) {
        fprintf(stderr, "XWM: current focus list is empty, shouldn't be\n");
        return;
    }
    do {
        if (p == client) {
#ifdef DEBUG
            printf("setting focus stack of workspace %d to %s\n",
                   client->workspace, client->name);
#endif /* DEBUG */
            focus_stacks[client->workspace - 1] = client;
            if (client->workspace == workspace_current) {
                focus_current = client;
                focus_ensure(timestamp);
            }
            return;
        }
        p = p->next_focus;
    } while (p != focus_stacks[client->workspace - 1]);
    fprintf(stderr, "XWM: client not found on focus list, shouldn't happen\n");
}

void focus_next(Time timestamp)
{
    if (focus_current != NULL)
        focus_set(focus_current->next_focus, timestamp);
}

void focus_prev(Time timestamp)
{
    if (focus_current != NULL)
        focus_set(focus_current->prev_focus, timestamp);
}

void focus_ensure(Time timestamp)
{
    if (focus_current == NULL) {
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
        return;
    }

#ifdef DEBUG
    printf("\tSetting focus to 0x%08X (%s)...\n",
           (unsigned int)focus_current->window, focus_current->name);
#endif /* DEBUG */

    /* see ICCCM 4.1.7 */
    if (focus_current->xwmh != NULL &&
        focus_current->xwmh->flags & InputHint &&
        focus_current->xwmh->input == False) {
        XSetInputFocus(dpy, root_window, RevertToPointerRoot, CurrentTime);
#ifdef DEBUG
        printf("\tdoesn't want focus\n");
#endif /* DEBUG */
        if (focus_current->protocols & PROTO_TAKE_FOCUS) {
            client_sendmessage(focus_current, WM_TAKE_FOCUS,
                               timestamp, 0, 0, 0);
#ifdef DEBUG
            printf("\tglobally active focus\n");
#endif /* DEBUG */
        }
    } else {
#ifdef DEBUG
        printf("\twants focus\n");
#endif /* DEBUG */
        if (focus_current->protocols & PROTO_TAKE_FOCUS) {
#ifdef DEBUG
            printf("\twill forcibly take focus\n");
#endif /* DEBUG */
            client_sendmessage(focus_current, WM_TAKE_FOCUS,
                               timestamp, 0, 0, 0);
        }
        XSetInputFocus(dpy, focus_current->window,
                       RevertToPointerRoot, timestamp);
    }
    XSync(dpy, False);
    XFlush(dpy);

    client_raise(focus_current);
}
