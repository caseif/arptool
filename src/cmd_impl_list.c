/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "arg_parse.h"
#include "cmd_impls.h"

#include "libarp/unpack.h"

#include <stdio.h>
#include <string.h>

#define MAX(a, b) ((a > b) ? (a) : (b))

int exec_cmd_list(arp_cmd_args_t *args) {
    char *src_path = args->src_path;

    ArgusPackage package;
    int rc = 0xDEADBEEF;
    if ((rc = load_package_from_file(src_path, &package)) != 0) {
        printf("Failed to load package (libarp says: %s)\n", libarp_get_error());
        return rc;
    }

    arp_resource_info_t *res_info;
    size_t res_count;
    if ((rc = list_resources(package, &res_info, &res_count)) != 0) {
        printf("Failed to list resources in package (libarp says: %s)\n", libarp_get_error());
        return rc;
    }

    size_t max_path = strlen("PATH");
    size_t max_ext = 0;
    size_t max_mt = strlen("TYPE");
    for (size_t i = 0; i < res_count; i++) {
        arp_resource_info_t *res = &res_info[i];
        max_path = MAX(max_path, strlen(res->path));
        max_ext = MAX(max_ext, strlen(res->extension));
        max_mt = MAX(max_mt, strlen(res->media_type));
    }

    printf("TYPE");

    for (size_t i = 0; i < max_mt - strlen("TYPE"); i++) {
        printf(" ");
    }

    printf("   PATH\n");

    for (size_t i = 0; i < max_mt + max_path + 3; i++) {
        printf("-");
    }
    printf("\n");

    for (size_t i = 0; i < res_count; i++) {
        arp_resource_info_t *res = &res_info[i];

        printf(res->media_type);

        size_t type_padding = max_mt - strlen(res->media_type);
        for (size_t i = 0; i < type_padding; i++) {
            printf(" ");
        }

        printf("   %s\n", res->path);
    }

    return 0;
}
