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

static int state1_transition_cb(struct xm_object *self, bool enter);

static int state2_process_cb(struct xm_object *self);

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_1",
                .id = STATE_1,
                .transition_cb = state1_transition_cb,
        },
        {
                .name = "STATE_2",
                .id = STATE_2,
                .process_cb = state2_process_cb,
        },
        {0}
};

static const struct xm_event_descriptor g_events_desc[] = {
        {0}
};

static int state1_transition_cb(struct xm_object *self, bool enter)
{
        if (enter != false)
                xm_state_request(self, STATE_2);
        
        return 0;
}

static int state2_process_cb(struct xm_object *self)
{
        xm_finish(self);

        return 0;
}

#define EVENTS_POOL_SIZE        8

static struct xm_event events_pool[EVENTS_POOL_SIZE];

static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_1,
        .name = "main",
        .events_pool = events_pool,
        .events_pool_size = EVENTS_POOL_SIZE,
};

struct xm_object g_fsm;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        xm_init(&g_fsm, &g_fsm_desc);
        while (xm_is_finish(&g_fsm) == false)
                xm_service(&g_fsm);

        return 0;
}
