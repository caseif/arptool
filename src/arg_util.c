/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "arg_parse.h"
#include "arg_util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

char *get_output_path(const arp_cmd_args_t *args, bool *malloced) {
    if (args->output_path != NULL) {
        *malloced = false;
        return args->output_path;
    } else {
        char *res = NULL;
        #ifdef _WIN32
        size_t required_size = GetCurrentDirectory(0, NULL);
        if ((res = malloc(required_size)) == NULL) {
            errno = ENOMEM;
            printf("Out of memory\n");
            return NULL;
        }
        *malloced = true;
        GetCurrentDirectory(required_size, res);
        return res;
        #else
        res = getcwd(NULL, 0);
        *malloced = true;
        return res;
        #endif
    }
}
