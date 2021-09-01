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

#include <stdio.h>
#include <string.h>

#include <unity.h>

#include "include/xm_api.h"

int g_assert_call_count;

struct xm_object g_self;
struct xm_object_descriptor g_desc;

int xm_event_manager_init_call_cntr;
int xm_state_manager_init_call_cntr;
xm_status_t xm_event_manager_init_return;
xm_status_t xm_state_manager_init_return;

int xm_event_process_call_cntr;
int xm_state_transition_call_cntr;
int xm_state_process_call_cntr;
int xm_state_finish_call_cntr;
xm_status_t xm_event_process_return;
xm_status_t xm_state_transition_return;
xm_status_t xm_state_process_return;
xm_status_t xm_state_finish_return;

int xm_mutex_lock_call_cntr;
int xm_mutex_unlock_call_cntr;

void setUp(void)
{
        g_assert_call_count = 0;
        
        memset((uint8_t*)&g_self, 0, sizeof(struct xm_object));
        memset((uint8_t*)&g_desc, 0, sizeof(struct xm_object_descriptor));

        xm_event_manager_init_call_cntr = 0;
        xm_state_manager_init_call_cntr = 0;
        xm_event_manager_init_return = XM_STATUS_OK;
        xm_state_manager_init_return = XM_STATUS_OK;

        xm_event_process_call_cntr = 0;
        xm_state_transition_call_cntr = 0;
        xm_state_process_call_cntr = 0;
        xm_state_finish_call_cntr = 0;

        xm_event_process_return = XM_STATUS_OK;
        xm_state_transition_return = XM_STATUS_OK;
        xm_state_process_return = XM_STATUS_OK;
        xm_state_finish_return = XM_STATUS_OK;

        xm_mutex_lock_call_cntr = 0;
        xm_mutex_unlock_call_cntr = 0;
}

void tearDown(void)
{

}

//////////////////////////
// MOCKS
//////////////////////////

xm_status_t xm_event_manager_init(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_event_manager_init_call_cntr;

        return xm_event_manager_init_return;
}

xm_status_t xm_state_manager_init(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_state_manager_init_call_cntr;

        return xm_state_manager_init_return;
}

xm_status_t xm_event_process(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_event_process_call_cntr;

        return xm_event_process_return;
}

xm_status_t xm_state_transition(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_state_transition_call_cntr;

        return xm_state_transition_return;
}

xm_status_t xm_state_process(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_state_process_call_cntr;

        return xm_state_process_return;
}

xm_status_t xm_state_finish(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_state_finish_call_cntr;

        return xm_state_finish_return;
}

void xm_mutex_lock(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_mutex_lock_call_cntr;
}

void xm_mutex_unlock(struct xm_object *self)
{
        TEST_ASSERT_EQUAL(&g_self, self);
        ++xm_mutex_unlock_call_cntr;
}

//////////////////////////

void test_xm_init(void)
{
        xm_init(&g_self, &g_desc);
        TEST_ASSERT_EQUAL(&g_desc, g_self.desc);
        TEST_ASSERT_EQUAL(1, xm_event_manager_init_call_cntr);
        TEST_ASSERT_EQUAL(1, xm_state_manager_init_call_cntr);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_event_manager_init_return);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_state_manager_init_return);
        TEST_ASSERT_EQUAL(0, xm_event_process_call_cntr);
        TEST_ASSERT_EQUAL(0, xm_state_transition_call_cntr);
        TEST_ASSERT_EQUAL(0, xm_state_process_call_cntr);
        TEST_ASSERT_EQUAL(0, xm_state_finish_call_cntr);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_event_process_return);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_state_transition_return);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_state_process_return);
        TEST_ASSERT_EQUAL(XM_STATUS_OK, xm_state_finish_return);
        TEST_ASSERT_EQUAL(1, xm_mutex_lock_call_cntr);
        TEST_ASSERT_EQUAL(1, xm_mutex_unlock_call_cntr);
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_xm_init);

        return UNITY_END();
}
