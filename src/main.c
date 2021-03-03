/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "cmdargs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    arp_cmd_args_t args;
    memset(&args, 0, sizeof(args));

    char *parse_err = parse_args(argc, argv, &args);

    if (parse_err != NULL) {
        printf("%s\n", parse_err);

        free(parse_err);

        exit(-1);
    }

    printf("compression: %s\n", args.compression);
    printf("name: %s\n", args.package_name);
    printf("namespace: %s\n", args.package_namespace);
    printf("mappings path: %s\n", args.mappings_path);
    printf("output path: %s\n", args.output_path);
    printf("part size: %lu\n", args.part_size);
    printf("target path: %s\n", args.target_path);
}
