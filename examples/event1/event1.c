#include "xm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

static bool g_exit_flag;
static int g_state2_cntr;

typedef enum {
        STATE_1,
        STATE_2,
} states_t;

typedef enum {
        EVENT_INIT,
        EVENT_1,
        EVENT_2,
        EVENT_FINISH,
} events_t;

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)arg;

        printf("APP: state1 event: %d\n", id);

        switch (id) {
        case EVENT_INIT:
                xm_event_trigger(self, EVENT_1, NULL);
                return 1;
        case EVENT_1:
                xm_state_request(self, STATE_2);
                return 1;
        default:
                return 0;
        }
        
        return 0;
}

static int state1_transition_cb(struct xm_object *self, bool enter)
{
        printf("APP: state1 transition: %s\n", (enter) ? "ENTER" : "EXIT");

        if (enter != false) {
                xm_event_trigger(self, EVENT_INIT, NULL);
                return 1;
        }
        
        return 0;
}

static int state1_process_cb(struct xm_object *self)
{
        (void)self;

        printf("APP: state1 process\n");
        return 0;
}

static int state2_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)self;
        (void)arg;

        printf("APP: state2 event: %d\n", id);

        switch (id) {
        case EVENT_2:
                printf("APP: nothing to do\n");
                return 0;
        case EVENT_FINISH:
                g_exit_flag = true;
                return 1;
        default:
                return 0;
        }
        
        return 0;
}

static int state2_transition_cb(struct xm_object *self, bool enter)
{
        (void)self;

        printf("APP: state2 transition: %s\n", (enter) ? "ENTER" : "EXIT");

        if (enter != false) {
                xm_event_trigger(self, EVENT_2, NULL);
                g_state2_cntr = 0;
                return 1;
        }

        return 0;
}

static int state2_process_cb(struct xm_object *self)
{
        (void)self;

        printf("APP: state2 process\n");
        if (++g_state2_cntr >= 10) 
                xm_event_trigger(self, EVENT_FINISH, NULL);

        return (g_state2_cntr > 5) ? 0 : 1;
}

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_1",
                .id = STATE_1,
                .event_cb = state1_event_cb,
                .transition_cb = state1_transition_cb,
                .process_cb = state1_process_cb,
        },
        {
                .name = "STATE_2",
                .id = STATE_2,
                .event_cb = state2_event_cb,
                .transition_cb = state2_transition_cb,
                .process_cb = state2_process_cb,
        },
        {0}
};

static const struct xm_event_descriptor g_events_desc[] = {
        {
                .name = "EVENT_INIT",
                .id = EVENT_INIT,
        },
        {
                .name = "EVENT_1",
                .id = EVENT_1,
        },
        {
                .name = "EVENT_2",
                .id = EVENT_2,
        },
        {
                .name = "EVENT_FINISH",
                .id = EVENT_FINISH,
        },
        {0}
};

static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_1,
        .name = "main_fsm"
};

struct xm_object g_fsm;

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        xm_init(&g_fsm, &g_fsm_desc);
        printf("APP: run\n");
        while (g_exit_flag == false) {
                xm_status_t s = xm_service(&g_fsm);
                printf("APP: service status: %d\n", s);
        }
        printf("APP: EXIT\n");
}
