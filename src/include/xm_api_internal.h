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

#ifndef XM_API_INTERNAL_H
#define XM_API_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xm_preconfig.h"
#include "xm_assert.h"
#include "xm_log.h"
#include "xm_heap.h"


xm_status_t             xm_event_manager_init(struct xm_object *self);
xm_status_t             xm_event_process(struct xm_object *self);
struct xm_event*        xm_event_malloc(struct xm_object *self);
void                    xm_event_free(struct xm_object *self, struct xm_event *event);

#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 1
struct xm_event*        xm_event_malloc_from_pool(struct xm_object *self);
void                    xm_event_free_from_pool(struct xm_object *self, struct xm_event *event);
#endif

xm_status_t             xm_state_manager_init(struct xm_object *self);
xm_status_t             xm_state_transition(struct xm_object *self);
xm_status_t             xm_state_finish(struct xm_object *self);
xm_status_t             xm_state_process(struct xm_object *self);

#ifdef __cplusplus
}
#endif

#endif /* XM_API_INTERNAL_H */
