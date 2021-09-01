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

#include <pthread.h>
#include <unistd.h>

typedef enum {
        STATE_COUNT,
        STATE_FINAL,
} states_t;

typedef enum {
        EVENT_1,
        EVENT_2,
        EVENT_FINISH,
} events_t;

static int g_event_cntr[2];

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);
static int state1_process_cb(struct xm_object *self);

static int state2_event_cb(struct xm_object *self, xm_event_id_t id, void *arg);
static int state2_transition_cb(struct xm_object *self, bool is_enter);

static const struct xm_state_descriptor g_states_desc[] = {
        {
                .name = "STATE_COUNT",
                .id = STATE_COUNT,
                .event_cb = state1_event_cb,
                .process_cb = state1_process_cb,
        },
        {
                .name = "STATE_FINAL",
                .id = STATE_FINAL,
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
        {
                .name = "EVENT_FINISH",
                .id = EVENT_FINISH,
        },
        {0}
};

static int state1_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)self;
        (void)arg;

        switch (id) {
        case EVENT_1:
                g_event_cntr[0]++;
                break;
        case EVENT_2:
                g_event_cntr[1]++;
                break;
        default:
                break;
        }
        
        return 0;
}

static int state1_process_cb(struct xm_object *self)
{
        int sum;

        sum = g_event_cntr[0] + g_event_cntr[1];
        if (sum >= 5)
                xm_state_request(self, STATE_FINAL);

        return 0;
}

static int state2_event_cb(struct xm_object *self, xm_event_id_t id, void *arg)
{
        (void)arg;

        switch (id) {
        case EVENT_FINISH:
                xm_finish(self);
                break;
        default:
                break;
        }
        
        return 0;
}


static int state2_transition_cb(struct xm_object *self, bool is_enter)
{
        if (is_enter)
                xm_event_trigger(self, EVENT_FINISH, NULL);
        
        return 0;
}

static pthread_mutex_t g_fsm_mutex = PTHREAD_MUTEX_INITIALIZER;

static int mutex_lock(struct xm_object *self, void *arg)
{
        (void)arg;

        pthread_mutex_lock(self->desc->mutex);
        return 0;
}

static int mutex_unlock(struct xm_object *self, void *arg)
{
        (void)arg;
        
        pthread_mutex_unlock(self->desc->mutex);
        return 0;
}

static const struct xm_object_descriptor g_fsm_desc = {
        .states = g_states_desc,
        .events = g_events_desc,
        .init_state_id = STATE_COUNT,
        .name = "main",
        .mutex = &g_fsm_mutex,
        .mutex_lock = mutex_lock,
        .mutex_unlock = mutex_unlock,
};

struct xm_object g_fsm;

void* fsm_service_task(void *ptr)
{
        (void)ptr;

        while (xm_is_finish(&g_fsm) == false) {
                xm_service(&g_fsm);
        }

        return NULL;
}

void* event_producer(void *ptr)
{
        events_t event_id = (events_t)ptr;

        while (xm_is_finish(&g_fsm) == false) {
                xm_event_trigger(&g_fsm, event_id, NULL);
                sleep(1);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        pthread_t thread[3];

        xm_init(&g_fsm, &g_fsm_desc);

        pthread_create(&thread[0], NULL, fsm_service_task, NULL);
        pthread_create(&thread[1], NULL, event_producer, (void*)EVENT_1);
        pthread_create(&thread[2], NULL, event_producer, (void*)EVENT_2);

        pthread_join(thread[0], NULL);
        pthread_join(thread[1], NULL); 
        pthread_join(thread[2], NULL); 

        return 0;
}
