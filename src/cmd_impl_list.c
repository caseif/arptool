/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "arg_parse.h"
#include "cmd_impls.h"
#include "misc_defines.h"
#include "util.h"

#include "arp/util/error.h"
#include "arp/unpack/list.h"
#include "arp/unpack/load.h"

#include <stdio.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define HEADER_TYPE "TYPE"
#define HEADER_PATH "PATH"

int exec_cmd_list(arp_cmd_args_t *args) {
    char *src_path = args->src_path;

    ArpPackage package = NULL;
    int rc = UNINIT_U32;
    if ((rc = arp_load_from_file(src_path, NULL, &package)) != 0) {
        arptool_print(args, LogLevelError, "Failed to load package (libarp says: %s)\n", arp_get_error());
        return rc;
    }

    arp_resource_listing_t *res_listings = NULL;
    size_t listing_count = 0;
    if ((rc = arp_get_resource_listing(package, &res_listings, &listing_count)) != 0) {
        arptool_print(args, LogLevelError, "Failed to list resources in package (libarp says: %s)\n", arp_get_error());
        return rc;
    }

    size_t max_path = strlen(HEADER_PATH);
    size_t max_ext = 0;
    size_t max_mt = strlen(HEADER_TYPE);
    for (size_t i = 0; i < listing_count; i++) {
        arp_resource_listing_t *listing = &res_listings[i];
        max_path = MAX(max_path, strlen(listing->path));
        max_ext = MAX(max_ext, strlen(listing->meta.extension));
        max_mt = MAX(max_mt, strlen(listing->meta.media_type));
    }

    arptool_print(args, LogLevelInfo, "%-*s   " HEADER_PATH "\n", (int) max_mt, HEADER_TYPE);

    if (args->verbosity == VerbosityNormal) {
        for (size_t i = 0; i < max_mt + max_path + 3; i++) {
            putchar('-');
        }

        putchar('\n');
    }

    for (size_t i = 0; i < listing_count; i++) {
        arp_resource_listing_t *listing = &res_listings[i];

        arptool_print(args, LogLevelInfo, "%-*s   %s\n", (int) max_mt, listing->meta.media_type, listing->path);
    }

    return 0;
}
