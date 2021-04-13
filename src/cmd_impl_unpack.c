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
#include "file_defines.h"

#include "libarp/unpack.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int exec_cmd_unpack(arp_cmd_args_t *args) {
    char *src_path = args->src_path;
    char *output_path = NULL;

    bool malloced_output_path = false;
    if ((output_path = get_output_path(args, &malloced_output_path)) == NULL) {
        return errno;
    }

    ArpPackage package;
    int rc = 0xDEADBEEF;
    if ((rc = load_package_from_file(src_path, &package)) != 0) {
        if (malloced_output_path) {
            free(output_path);
        }

        printf("Failed to load package (rc: %d) (libarp says: %s)\n", rc, libarp_get_error());
        return rc;
    }

    printf("Successfully loaded package\n");

    if (args->resource_path != NULL) {
        arp_resource_meta_t meta;
        if ((rc = get_resource_meta(package, args->resource_path, &meta)) != 0) {
            if (malloced_output_path) {
                free(output_path);
            }

            printf("Failed to get resource meta (libarp says: %s)\n", libarp_get_error());
            return rc;
        }

        arp_resource_t *res;
        if ((res = load_resource(&meta)) == NULL) {
            if (malloced_output_path) {
                free(output_path);
            }

            printf("Failed to load resource (libarp says: %s)\n", libarp_get_error());
            return errno;
        }

        size_t res_op_len_s = strlen(output_path) + 1 + strlen(res->meta.base_name) + 1 + strlen(res->meta.extension);
        size_t res_op_len_b = res_op_len_s + 1;
        char *res_output_path = NULL;
        if ((res_output_path = malloc(res_op_len_b)) == NULL) {
            if (malloced_output_path) {
                free(output_path);
            }

            printf("Out of memory\n");
            return ENOMEM;
        }

        if (res->meta.extension != NULL && strlen(res->meta.extension) > 0) {
            snprintf(res_output_path, res_op_len_b, "%s%c%s%c%s",
                    output_path, PATH_DELIMITER, res->meta.base_name, EXTENSION_DELIMITER, res->meta.extension);
        } else {
            snprintf(res_output_path, res_op_len_b, "%s%c%s",
                    output_path, PATH_DELIMITER, res->meta.base_name);
        }

        FILE *output_file = NULL;
        if ((output_file = fopen(res_output_path, "w+b")) == NULL) {
            free(res_output_path);

            if (malloced_output_path) {
                free(output_path);
            }

            printf("Failed to open output file\n");
            return errno;
        }

        free(res_output_path);

        if (fwrite(res->data, res->meta.size, 1, output_file) != 1) {
            fclose(output_file);

            if (malloced_output_path) {
                free(output_path);
            }

            printf("Failed to write to output file\n");
            return errno;
        }

        fclose(output_file);

        printf("Successfully unpacked %s to disk\n", args->resource_path);

        rc = 0;
    } else {
        if ((rc = unpack_arp_to_fs(package, output_path)) == 0) {
            printf("Successfully unpacked package to disk!\n");
        } else {
            printf("Failed to unpack package to disk (rc: %d) (libarp says: %s)\n", rc, libarp_get_error());
        }
    }

    if (malloced_output_path) {
        free(output_path);
    }

    return rc;
}
