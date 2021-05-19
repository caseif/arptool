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
#include "cmd_impls.h"
#include "compression_defines.h"
#include "file_defines.h"
#include "misc_defines.h"

#include "libarp/defines.h"
#include "libarp/pack.h"

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <shlwapi.h>
#include <winbase.h>
#else
#include <unistd.h>
#endif

int exec_cmd_pack(arp_cmd_args_t *args) {
    char *src_path = args->src_path;
    char *output_path = NULL;
    char *package_name = args->package_name;
    char package_namespace[ARP_NAMESPACE_MAX + 1];
    char *mappings_path = args->mappings_path;
    char *compression_magic = NULL;
    size_t part_size = args->part_size;

    bool malloced_output_path = false;
    if ((output_path = get_output_path(args, &malloced_output_path)) == NULL) {
        return errno;
    }

    if (package_name == NULL) {
        char *delim = strrchr(src_path, PATH_DELIMITER);
        if (delim != NULL) {
            package_name = delim + 1;
        } else {
            package_name = src_path;
        }
    }

    if (args->package_namespace != NULL) {
        strncpy(package_namespace, args->package_namespace, sizeof(package_namespace));
        package_namespace[strlen(args->package_namespace)] = '\0';
    } else {
        strncpy(package_namespace, package_name, sizeof(package_namespace));
        package_namespace[strlen(package_name)] = '\0';
    }

    if (args->compression != NULL) {
        if (strcmp(args->compression, CMPR_STR_DEFLATE) == 0) {
            compression_magic = ARP_COMPRESS_TYPE_DEFLATE;
        } else if (strcmp(args->compression, CMPR_STR_NONE) == 0) {
            // no-op
        } else {
            if (malloced_output_path) {
                free(output_path);
            }

            printf("Unrecognized compression type\n");
            return EINVAL;
        }
    }

    ArpPackingOptions opts = create_v1_packing_options(package_name, package_namespace, part_size, compression_magic,
            mappings_path);

    int rc = UNINIT_U32;
    if ((rc = create_arp_from_fs(src_path, output_path, opts, NULL)) == 0) {
        printf("Successfully wrote archive to %s\n", output_path);
    } else {
        printf("Packing failed\n");
        printf("libarp says: %s (rc: %d)\n", libarp_get_error(), rc);
    }

    free_packing_options(opts);

    if (malloced_output_path) {
        free(output_path);
    }

    return rc;
}
