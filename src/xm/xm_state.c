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

        XM_ASSERT(self != NULL);
        XM_ASSERT(self->desc->states != NULL);

        self->state.current = NULL;
        self->state.max_num = 0;
        self->state.request_id = XM_STATE_NO_STATE;
        while (self->desc->states[self->state.max_num].name != NULL)
                self->state.max_num++;

        if (self->state.max_num == 0) {
                XM_LOG_E("empty states table");
                ret = XM_STATUS_ERROR_EMPTY_STATES_TABLE;
                goto _xm_state_manager_init_return;
        }

        ret = xm_state_request(self, self->desc->init_state_id);

_xm_state_manager_init_return:
        return ret;
}

xm_status_t xm_state_request(struct xm_object *self, xm_state_id_t id)
{
        xm_status_t ret = XM_STATUS_OK;

        XM_ASSERT(self != NULL);
        XM_ASSERT(id >= 0);

        if (id >= self->state.max_num) {
                XM_LOG_E("unsupported state: %d", id);
                ret = XM_STATUS_ERROR_UNSUPPORTED_STATE;
                goto _xm_state_request_return;
        }

        self->state.request_id = id;
        XM_LOG_I("request state: <%s>", self->desc->states[self->state.request_id].name);

_xm_state_request_return:
        return ret;
}

xm_status_t xm_state_transition(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        
        XM_ASSERT(self != NULL);

        if (self->state.request_id < 0) {
                ret = XM_STATUS_ERROR_NO_PENDING_TRANSITION;
                goto _xm_state_transition_return;
        }

        if (self->state.current == NULL) {
                XM_LOG_I("initial state");
                goto _xm_state_transition_enter;
        }

        if (self->state.current->id == self->state.request_id) {
                XM_LOG_E("already active: <%s>", self->state.current->name);
                ret = XM_STATUS_ERROR_STATE_ALREADY_ACTIVE;
                goto _xm_state_transition_return;
        }
        XM_LOG_I("exit: <%s>", self->state.current->name);
        int busy = 0;
        if (self->state.current->transition_cb != NULL)
                busy = self->state.current->transition_cb(self, false);
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;

_xm_state_transition_enter:

        self->state.current = &self->desc->states[self->state.request_id];
        XM_LOG_I("enter: <%s>", self->state.current->name);
        if (self->state.current->transition_cb != NULL)
                self->state.current->transition_cb(self, true);

_xm_state_transition_return:
        self->state.request_id = XM_STATE_NO_STATE;
        return ret;
}

xm_status_t xm_state_process(struct xm_object *self)
{
        xm_status_t ret = XM_STATUS_OK;
        
        XM_ASSERT(self != NULL);

        if (self->state.current == NULL) {
                XM_LOG_E("no active state");
                ret = XM_STATUS_ERROR_NO_ACTIVE_STATE;
                goto _xm_state_process_return;
        }

        int busy = 0;
        if (self->state.current->process_cb != NULL)
                busy = self->state.current->process_cb(self);
        ret = (busy != 0) ? XM_STATUS_OK : XM_STATUS_ERROR_NOTHING_TO_DO;

_xm_state_process_return:
        return ret;
}
