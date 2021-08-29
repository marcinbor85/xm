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

#ifndef XM_CONFIG_H
#define XM_CONFIG_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "xm_log.h"

__attribute__((weak)) const char *log_level_name[] = {
    [XM_LOG_LEVEL_DEBUG] = "D",
    [XM_LOG_LEVEL_INFO] = "I",
    [XM_LOG_LEVEL_ERROR] = "E",
};

#define XM_CONFIG_ASSERT(x)                         assert(x)
#define XM_CONFIG_LOG(self, level, fmt, ...)        printf("xm: %s: [%s]: " fmt "\n", self->desc->name, log_level_name[level], ##__VA_ARGS__)
#define XM_CONFIG_MALLOC(n)                         malloc(n)
#define XM_CONFIG_FREE(p)                           free(p)

#endif /* XM_CONFIG_H */
