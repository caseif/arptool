/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#pragma once

#include <stddef.h>

typedef struct ArpCmdArgs {
    char *verb;
    char *target_path;
    char *compression;
    char *mappings_path;
    char *package_name;
    char *package_namespace;
    char *output_path;
    size_t part_size;
} arp_cmd_args_t;

char *parse_args(int argc, char **argv, arp_cmd_args_t *out_args);
