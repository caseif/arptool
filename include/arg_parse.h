/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum Verbosity {
    VerbosityNormal,
    VerbosityQuiet,
    VerbositySilent
};

typedef struct ArpCmdArgs {
    bool is_help;
    enum Verbosity verbosity;

    char *verb;
    char *src_path;
    char *compression;
    char *mappings_path;
    char *package_name;
    char *package_namespace;
    char *output_path;
    uint64_t part_size;
    char *resource_path;
} arp_cmd_args_t;

char *parse_args(int argc, char **argv, arp_cmd_args_t *out_args);
