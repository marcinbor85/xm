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

#ifndef XM_STATE_H
#define XM_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xm_types.h"

struct xm_state_descriptor {
        const char *name;
        xm_state_id_t id;
        
        xm_state_event_cb event_cb;
        xm_state_transition_cb transition_cb;
        xm_state_process_cb process_cb;
};

struct xm_state_manager {
        xm_state_id_t request_id;
        xm_state_id_t max_num;

        bool finish_request;
        
        struct xm_state_descriptor const *current;
};

#ifdef __cplusplus
}
#endif

#endif /* XM_STATE_H */
