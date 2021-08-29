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

#ifndef XM_OBJECT_H
#define XM_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xm_types.h"

#include "xm_state.h"
#include "xm_event.h"

struct xm_object_descriptor {
        struct xm_state_descriptor const *states;
        struct xm_event_descriptor const *events;

        xm_state_id_t init_state_id;
        const char *name;
};

struct xm_object {
        struct xm_object_descriptor const *desc;

        struct xm_state_manager state;
        struct xm_event_manager event;
};

#ifdef __cplusplus
}
#endif

#endif /* XM_OBJECT_H */
