#include "arg_parse.h"
#include "arg_util.h"
#include "cmd_impls.h"

#include "libarp/unpack.h"

#include <stdbool.h>
#include <stdio.h>

void exec_cmd_unpack(arp_cmd_args_t *args) {
    char *src_path = args->src_path;
    char *output_path = NULL;

    bool malloced_output_path = false;
    if ((output_path = get_output_path(args, &malloced_output_path)) == NULL) {
        return;
    }

    ArgusPackage package;
    if (load_package_from_file(src_path, &package) != 0) {
        free(output_path);

        printf("Failed to load package (libarp says: %s)\n", libarp_get_error());
        return;
    }

    printf("Successfully loaded package\n");

    if (malloced_output_path) {
        free(output_path);
    }
}
