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

#ifndef XM_API_H
#define XM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xm_types.h"

#include "xm_object.h"
#include "xm_event.h"
#include "xm_state.h"

xm_status_t     xm_init(struct xm_object *self, const struct xm_object_descriptor *desc);
xm_status_t     xm_deinit(struct xm_object *self);
xm_status_t     xm_service(struct xm_object *self);
xm_status_t     xm_event_trigger(struct xm_object *self, xm_event_id_t id, void *arg);
xm_status_t     xm_state_request(struct xm_object *self, xm_state_id_t id);
xm_state_id_t   xm_state_get_current(struct xm_object *self);
xm_status_t     xm_finish(struct xm_object *self);
bool            xm_is_finish(struct xm_object *self);

#if XM_CONFIG_FLAG_STATIC_ALLOCATION == 0
struct xm_object*       xm_new(const struct xm_object_descriptor *desc);
void                    xm_delete(struct xm_object* self);
#endif

#ifdef __cplusplus
}
#endif

#endif /* XM_API_H */
