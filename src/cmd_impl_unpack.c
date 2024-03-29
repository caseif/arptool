/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "arg_parse.h"
#include "arg_util.h"
#include "cmd_impls.h"
#include "file_defines.h"
#include "misc_defines.h"
#include "util.h"

#include "arp/unpack/load.h"
#include "arp/unpack/resource.h"
#include "arp/unpack/unpack.h"
#include "arp/util/error.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int exec_cmd_unpack(arp_cmd_args_t *args) {
    char *src_path = args->src_path;
    char *output_path = NULL;

    bool malloced_output_path = false;
    if ((output_path = get_output_path(args, &malloced_output_path)) == NULL) {
        return errno;
    }

    ArpPackage package = NULL;
    int rc = UNINIT_U32;
    if ((rc = arp_load_from_file(src_path, NULL, &package)) != 0) {
        if (malloced_output_path) {
            free(output_path);
        }

        arptool_print(args, LogLevelError, "Failed to load package (rc: %d) (libarp says: %s)\n", rc, arp_get_error());
        return rc;
    }

    arptool_print(args, LogLevelInfo, "Successfully loaded package\n");

    if (args->resource_path != NULL) {
        arp_resource_meta_t meta;
        if ((rc = arp_find_resource(package, args->resource_path, &meta)) != 0) {
            arp_unload(package);

            if (malloced_output_path) {
                free(output_path);
            }

            arptool_print(args, LogLevelError, "Failed to get resource meta (libarp says: %s)\n", arp_get_error());
            return rc;
        }

        rc = arp_unpack_resource_to_fs(&meta, output_path);

        if (rc == 0) {
            arptool_print(args, LogLevelInfo, "Successfully unpacked %s to disk\n", args->resource_path);
        } else {
            arptool_print(args, LogLevelError, "Failed to unpack resource to disk (libarp says: %s)\n",
                    arp_get_error());
        }


        rc = 0;
    } else {
        if ((rc = arp_unpack_to_fs(package, output_path)) == 0) {
            arptool_print(args, LogLevelInfo, "Successfully unpacked package to disk!\n");
        } else {
            arptool_print(args, LogLevelError, "Failed to unpack package to disk (rc: %d) (libarp says: %s)\n",
                    rc, arp_get_error());
        }
    }

    arp_unload(package);

    if (malloced_output_path) {
        free(output_path);
    }

    return rc;
}
