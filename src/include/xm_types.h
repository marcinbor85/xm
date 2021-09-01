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

#ifndef XM_TYPES_H
#define XM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xm_const.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef int xm_event_id_t;
typedef int xm_state_id_t;

struct xm_object;
struct xm_object_descriptor;

struct xm_event;
struct xm_event_descriptor;
struct xm_event_manager;

struct xm_state_descriptor;
struct xm_state_manager;

typedef int (*xm_callable_iface)(struct xm_object *self, void *arg);

typedef int (*xm_state_event_cb)(struct xm_object *self, xm_event_id_t id, void *arg);
typedef int (*xm_state_transition_cb)(struct xm_object *self, bool enter);
typedef int (*xm_state_process_cb)(struct xm_object *self);

typedef enum {
    XM_STATUS_OK,
    XM_STATUS_ERROR_MEMORY_ALLOCATION,
    XM_STATUS_ERROR_NO_PENDING_EVENT,
    XM_STATUS_ERROR_NO_PENDING_TRANSITION,
    XM_STATUS_ERROR_NO_ACTIVE_STATE,
    XM_STATUS_ERROR_STATE_ALREADY_ACTIVE,
    XM_STATUS_ERROR_STATE_ALREADY_FINISHED,
    XM_STATUS_ERROR_UNSUPPORTED_STATE,
    XM_STATUS_ERROR_UNSUPPORTED_EVENT,
    XM_STATUS_ERROR_EMPTY_STATES_TABLE,
    XM_STATUS_ERROR_NO_STATE_TO_PROCESS_EVENT,
    XM_STATUS_ERROR_NOTHING_TO_DO,
} xm_status_t;

#ifdef __cplusplus
}
#endif

#endif /* XM_TYPES_H */
