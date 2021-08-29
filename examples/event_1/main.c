#include "xm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

static bool g_exit_flag;

typedef enum {
        STATE_1,
        STATE_2,
} states_t;

typedef enum {
        EVENT_1,
        EVENT_2,
} events_t;

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);
static int state1_transition_cb(struct xm_object *self, bool enter);

static int state2_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);
static int state2_transition_cb(struct xm_object *self, bool enter);

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_1",
                .id = STATE_1,
                .event_cb = state1_event_cb,
                .transition_cb = state1_transition_cb,
        },
        {
                .name = "STATE_2",
                .id = STATE_2,
                .event_cb = state2_event_cb,
                .transition_cb = state2_transition_cb,
        },
        {0}
};

static const struct xm_event_descriptor g_events_desc[] = {
        {
                .name = "EVENT_1",
                .id = EVENT_1,
        },
        {
                .name = "EVENT_2",
                .id = EVENT_2,
        },
        {0}
};

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)arg;

        switch (id) {
        case EVENT_1:
                xm_state_request(self, STATE_2);
                break;
        default:
                break;
        }
        
        return 0;
}

static int state1_transition_cb(struct xm_object *self, bool enter)
{
        if (enter != false)
                xm_event_trigger(self, EVENT_1, NULL);
        
        return 0;
}

static int state2_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)self;
        (void)arg;

        switch (id) {
        case EVENT_2:
                g_exit_flag = true;
                break;
        default:
                break;
        }
        
        return 0;
}

static int state2_transition_cb(struct xm_object *self, bool enter)
{
        if (enter != false) {
                xm_event_trigger(self, EVENT_2, NULL);
        }
        
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
        while (g_exit_flag == false)
                xm_service(&g_fsm);
}
