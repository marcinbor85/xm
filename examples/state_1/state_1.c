#include "xm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

static bool g_exit_flag;

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
        if (enter != false) {
                xm_state_request(self, STATE_2);
        }
        
        return 0;
}

static int state2_process_cb(struct xm_object *self)
{
        (void)self;

        g_exit_flag = true;

        return 0;
}

static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_1,
        .name = "main"
};

struct xm_object g_fsm;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        xm_init(&g_fsm, &g_fsm_desc);
        while (g_exit_flag == false) {
                xm_service(&g_fsm);
        }
}
