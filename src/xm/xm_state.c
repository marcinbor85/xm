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

xm_status_t xm_state_manager_init(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        struct xm_state_manager *mgr = &self->state;

        XM_ASSERT(self != NULL);
        XM_ASSERT(self->desc->states != NULL);

        mgr->current = NULL;
        mgr->request_id = XM_ID_NOT_USED;
        mgr->finish_request = false;
        mgr->max_num = 0;
        while (self->desc->states[mgr->max_num].name != NULL)
                mgr->max_num++;

        if (mgr->max_num == 0) {
                XM_LOG_E("empty states table");
                ret = XM_STATUS_ERROR_EMPTY_STATES_TABLE;
                goto _xm_state_manager_init_return;
        }

        xm_mutex_unlock(self);
        ret = xm_state_request(self, self->desc->init_state_id);
        xm_mutex_lock(self);

_xm_state_manager_init_return:
        return ret;
}

xm_status_t xm_state_request(struct xm_object *self, xm_state_id_t id)
{
        xm_status_t ret = XM_STATUS_OK;

        XM_ASSERT(self != NULL);
        XM_ASSERT(id >= 0);

        xm_mutex_lock(self);

        struct xm_state_manager *mgr = &self->state;

        if (id >= mgr->max_num) {
                XM_LOG_E("unsupported state: %d", id);
                ret = XM_STATUS_ERROR_UNSUPPORTED_STATE;
                goto _xm_state_request_return;
        }

        mgr->request_id = id;
        XM_LOG_I("request state: <%s>", self->desc->states[mgr->request_id].name);

_xm_state_request_return:

        xm_mutex_unlock(self);
        return ret;
}

xm_status_t xm_state_transition(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        const struct xm_state_descriptor *current;
        
        XM_ASSERT(self != NULL);

        struct xm_state_manager *mgr = &self->state;
        xm_state_id_t request_id = mgr->request_id;

        if (request_id < 0) {
                ret = XM_STATUS_ERROR_NO_PENDING_TRANSITION;
                goto _xm_state_transition_return;
        }
        mgr->request_id = XM_ID_NOT_USED;

        current = mgr->current;
        if (current == NULL) {
                XM_LOG_I("init");
                goto _xm_state_transition_enter;
        }

        if (current->id == request_id) {
                XM_LOG_E("already active: <%s>", current->name);
                ret = XM_STATUS_ERROR_STATE_ALREADY_ACTIVE;
                goto _xm_state_transition_return;
        }
        XM_LOG_I("exit: <%s>", current->name);
        int busy = 0;
        if (current->transition_cb != NULL) {
                xm_mutex_unlock(self);
                busy = current->transition_cb(self, false);
                xm_mutex_lock(self);
        }
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;

_xm_state_transition_enter:

        mgr->current = &self->desc->states[request_id];
        current = mgr->current;
        XM_LOG_I("enter: <%s>", current->name);
        if (current->transition_cb != NULL) {
                xm_mutex_unlock(self);
                current->transition_cb(self, true);
                xm_mutex_lock(self);
        }

_xm_state_transition_return:
        return ret;
}

xm_status_t xm_state_process(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        
        XM_ASSERT(self != NULL);

        struct xm_state_manager *mgr = &self->state;
        const struct xm_state_descriptor *current = mgr->current;

        if (current == NULL) {
                XM_LOG_E("no active state");
                ret = XM_STATUS_ERROR_NO_ACTIVE_STATE;
                goto _xm_state_process_return;
        }

        int busy = 0;
        if (current->process_cb != NULL) {
                xm_mutex_unlock(self);
                busy = current->process_cb(self);
                xm_mutex_lock(self);
        }
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;

_xm_state_process_return:
        return ret;
}

xm_status_t xm_state_finish(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;

        XM_ASSERT(self != NULL);
        struct xm_state_manager *mgr = &self->state;

        if (mgr->finish_request == false) {
                ret = XM_STATUS_ERROR_NOTHING_TO_DO;
                goto _xm_state_finish_return;
        }

        const struct xm_state_descriptor *current = mgr->current;
        if (current == NULL) {
                XM_LOG_E("already finished");
                ret = XM_STATUS_ERROR_STATE_ALREADY_FINISHED;
                goto _xm_state_finish_return;
        }
        XM_LOG_I("exit: <%s>", current->name);
        int busy = 0;
        if (current->transition_cb != NULL) {
                xm_mutex_unlock(self);
                busy = current->transition_cb(self, false);
                xm_mutex_lock(self);
        }
        XM_LOG_I("finish");
        mgr->current = NULL;
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;

_xm_state_finish_return:
        return ret;
}
