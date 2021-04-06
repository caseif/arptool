#include "arg_parse.h"
#include "arg_util.h"
#include "cmd_impls.h"

#include "libarp/unpack.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

int exec_cmd_unpack(arp_cmd_args_t *args) {
    char *src_path = args->src_path;
    char *output_path = NULL;

    bool malloced_output_path = false;
    if ((output_path = get_output_path(args, &malloced_output_path)) == NULL) {
        return errno;
    }

    ArgusPackage package;
    int rc = 0xDEADBEEF;
    if ((rc = load_package_from_file(src_path, &package)) != 0) {
        if (malloced_output_path) {
            free(output_path);
        }

        printf("Failed to load package (rc: %d) (libarp says: %s)\n", rc, libarp_get_error());
        return rc;
    }

    printf("Successfully loaded package\n");

    if ((rc = unpack_arp_to_fs(package, output_path)) == 0) {
        printf("Successfully unpacked package to disk!\n");
    } else {
        if (malloced_output_path) {
            free(output_path);
        }

        printf("Failed to unpack package to disk (rc: %d) (libarp says: %s)\n", rc, libarp_get_error());
    }

    if (malloced_output_path) {
        free(output_path);
    }

    return rc;
}
