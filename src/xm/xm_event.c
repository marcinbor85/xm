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

#include "xm_api.h"
#include "xm_api_internal.h"

xm_status_t xm_event_manager_init(struct xm_object *self)
{
        struct xm_event_manager *mgr = &self->event;

        XM_ASSERT(self != NULL);
        XM_ASSERT(self->desc->events != NULL);

        mgr->first = NULL;
        mgr->last = &mgr->first;
        mgr->max_num = 0;
        while (self->desc->events[mgr->max_num].name != NULL)
                mgr->max_num++;

#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 1
        XM_ASSERT(self->desc->events_pool != NULL);
        XM_ASSERT(self->desc->events_pool_size > 0);

        mgr->event_pool_index = 0;
        for (size_t i = 0; i < self->desc->events_pool_size; i++) {
                struct xm_event *event = &self->desc->events_pool[i];
                event->id = XM_ID_NOT_USED;
        }
#endif
        
        return XM_STATUS_OK;
}

#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 1

struct xm_event* xm_event_malloc_from_pool(struct xm_object *self)
{
        struct xm_event *ret = NULL;
        struct xm_event_manager *mgr = &self->event;
        size_t pool_size = self->desc->events_pool_size;

        for (size_t i = 0; i < pool_size; i++) {
                size_t idx = (mgr->event_pool_index + i) % pool_size;
                struct xm_event *event = &self->desc->events_pool[idx];
                if (event->id == XM_ID_NOT_USED) {
                        ret = event;
                        mgr->event_pool_index = (idx + 1) % pool_size;
                        break;
                }
        }

        return ret;
}

void xm_event_free_from_pool(struct xm_object *self, struct xm_event *event)
{
        (void)self;
        
        event->id = XM_ID_NOT_USED;
}

#endif

struct xm_event* xm_event_malloc(struct xm_object *self)
{
        struct xm_event *event = NULL;
#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 0
        (void)self;
        event = XM_MALLOC(sizeof(struct xm_event));
#else
        event = xm_event_malloc_from_pool(self);
#endif
        return event;
}

void xm_event_free(struct xm_object *self, struct xm_event *event)
{        
#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 0
        (void)self;
        XM_FREE(event);
#else
        xm_event_free_from_pool(self, event);
#endif
}

xm_status_t xm_event_trigger(struct xm_object *self, xm_event_id_t id, void *arg)
{
        xm_status_t ret = XM_STATUS_OK;
        struct xm_event_manager *mgr = &self->event;

        XM_ASSERT(self != NULL);
        XM_ASSERT(id >= 0);

        if (id >= mgr->max_num) {
                XM_LOG_E("unsupported event: %d", id);
                ret = XM_STATUS_ERROR_UNSUPPORTED_EVENT;
                goto _xm_event_trigger_return;
        }

        struct xm_event *event = xm_event_malloc(self);
        if (event == NULL) {
                XM_LOG_E("memory allocation error");
                ret = XM_STATUS_ERROR_MEMORY_ALLOCATION;
                goto _xm_event_trigger_return;
        }

        const struct xm_state_descriptor *state = self->state.current;

        event->id = id;
        event->arg = arg;
        event->state_id = (state != NULL) ? state->id : XM_ID_NOT_USED;
        event->next = NULL;

        *mgr->last = event;
        mgr->last = &event->next;

        XM_LOG_I("event triggered: [%s]", self->desc->events[id].name);

_xm_event_trigger_return:
        return ret;
}

xm_status_t xm_event_process(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        struct xm_event_manager *mgr = &self->event;

        XM_ASSERT(self != NULL);

        struct xm_event *event = mgr->first;

        if (event == NULL) {
                ret = XM_STATUS_ERROR_NO_PENDING_EVENT;
                goto _xm_event_process_return;
        }

        if (event->state_id < 0) {
                XM_LOG_E("no state to process event");
                ret = XM_STATUS_ERROR_NO_STATE_TO_PROCESS_EVENT;
                goto _xm_event_process_end;
        }
        
        const struct xm_state_descriptor *state = &self->desc->states[event->state_id];
        if (state->event_cb == NULL)
                goto _xm_event_process_end;
        
        int busy = state->event_cb(self, event->id, event->arg);
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;
        
_xm_event_process_end:
        mgr->first = event->next;
        if (mgr->first == NULL)
                mgr->last = &mgr->first;

        XM_LOG_I("event processed: [%s]", self->desc->events[event->id].name);
        xm_event_free(self, event);

_xm_event_process_return:
        return ret;
}
