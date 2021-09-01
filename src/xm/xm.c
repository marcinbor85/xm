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

xm_status_t xm_init(struct xm_object *self, const struct xm_object_descriptor *desc)
{
        xm_status_t ret = XM_STATUS_OK;

        XM_ASSERT(self != NULL);
        XM_ASSERT(desc != NULL);

        self->desc = desc;

        xm_mutex_lock(self);

        ret = xm_event_manager_init(self);
        if (ret != XM_STATUS_OK) {
                XM_LOG_E("xm_event_manager_init error");
                goto _xm_init_return;
        }

        ret = xm_state_manager_init(self);
        if (ret != XM_STATUS_OK) {
                XM_LOG_E("xm_state_manager_init error");
                goto _xm_init_return;
        }

_xm_init_return:

        xm_mutex_unlock(self);

        return ret;
}

xm_status_t xm_deinit(struct xm_object *self)
{
        XM_ASSERT(self != NULL);
        
        xm_mutex_lock(self);

        struct xm_event_manager *mgr = &self->event;
        struct xm_event *event = mgr->first;

        while (event != NULL) {
                struct xm_event *next = event->next;
                xm_event_free(self, event);
                event = next;
        }

        xm_mutex_unlock(self);

        return XM_STATUS_OK;
}

xm_status_t xm_service(struct xm_object *self)
{
        XM_ASSERT(self != NULL);

        xm_status_t ret = XM_STATUS_ERROR_NOTHING_TO_DO;

        xm_mutex_lock(self);

        ret = xm_event_process(self);
        if (ret == XM_STATUS_OK)
                goto _xm_service_return;

        ret = xm_state_transition(self);
        if (ret == XM_STATUS_OK)
                goto _xm_service_return;

        ret = xm_state_process(self);
        if (ret == XM_STATUS_OK)
                goto _xm_service_return;

        ret = xm_state_finish(self);
        if (ret == XM_STATUS_OK)
                goto _xm_service_return;

_xm_service_return:

        xm_mutex_unlock(self);

        return ret;
}

xm_status_t xm_finish(struct xm_object *self)
{
        XM_ASSERT(self != NULL);

        xm_mutex_lock(self);
        self->state.finish_request = true;
        xm_mutex_unlock(self);

        return XM_STATUS_OK;
}

bool xm_is_finish(struct xm_object *self)
{
        bool ret;

        XM_ASSERT(self != NULL);

        xm_mutex_lock(self);
        ret = ((self->state.current == NULL) && (self->state.finish_request != false)) ? true : false;
        xm_mutex_unlock(self);

        return ret;
}

#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 0

struct xm_object* xm_new(const struct xm_object_descriptor *desc)
{
        XM_ASSERT(desc != NULL);
        
        struct xm_object* ret;

        ret = XM_MALLOC(sizeof(struct xm_object));
        if (ret != NULL)
                (void)xm_init(ret, desc);

        return ret;
}

void xm_delete(struct xm_object* self)
{
        XM_ASSERT(self != NULL);

        xm_deinit(self);

        XM_FREE(self);
}

#endif