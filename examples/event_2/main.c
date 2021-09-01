/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "xm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {
        STATE_1,
        STATE_2,
} states_t;

typedef enum {
        EVENT_ACCEPT,
        EVENT_FINISH,
} events_t;

static int common_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);

static int state1_transition_cb(struct xm_object *self, bool enter);
static int state2_transition_cb(struct xm_object *self, bool enter);

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_1",
                .id = STATE_1,
                .event_cb = common_event_cb,
                .transition_cb = state1_transition_cb,
        },
        {
                .name = "STATE_2",
                .id = STATE_2,
                .event_cb = common_event_cb,
                .transition_cb = state2_transition_cb,
        },
        {0}
};

static const struct xm_event_descriptor g_events_desc[] = {
        {
                .name = "EVENT_ACCEPT",
                .id = EVENT_ACCEPT,
        },
        {
                .name = "EVENT_FINISH",
                .id = EVENT_FINISH,
        },
        {0}
};

static int common_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)arg;

        switch (id) {
        case EVENT_ACCEPT:
                xm_state_request(self, STATE_2);
                break;
        case EVENT_FINISH:
                if (xm_state_get_current(self) == STATE_2)
                        xm_finish(self);
                break;
        default:
                break;
        }
        
        return 0;
}

static int state1_transition_cb(struct xm_object *self, bool enter)
{
        if (enter != false) {
                xm_event_trigger(self, EVENT_FINISH, NULL);
                xm_event_trigger(self, EVENT_ACCEPT, NULL);
        }
        
        return 0;
}

static int state2_transition_cb(struct xm_object *self, bool enter)
{
        if (enter != false)
                xm_event_trigger(self, EVENT_FINISH, NULL);
        
        return 0;
}

static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_1,
        .name = "main"
};

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        struct xm_object *fsm = xm_new(&g_fsm_desc);
        while (xm_is_finish(fsm) == false)
                xm_service(fsm);
        xm_delete(fsm);

        return 0;
}
