/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg_defines.h"
#include "cmdargs.h"
#include "compression_defines.h"

#define ERR_MSG_BUF_LEN 256

#define PARSE_FAIL(format, ...) { \
    char *_cpp_err_msg_buf = malloc(ERR_MSG_BUF_LEN); \
    snprintf(_cpp_err_msg_buf, ERR_MSG_BUF_LEN, format, ##__VA_ARGS__); \
    return _cpp_err_msg_buf; \
}

char *parse_args(int argc, char **argv, arp_cmd_args_t *out_args) {
    size_t pos = 0;
    bool stopped_args = false;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        
        if (strcmp(arg, "--") == 0) {
            // done argument parsing
            stopped_args = true;
            continue;
        }

        if (!stopped_args) {
            if (strncmp(arg, "--", 2) == 0) {
                char *flag = arg + 2;

                if (strcmp(flag, NFLAG_DEFLATE) == 0) {
                    out_args->compression = CMPR_STR_DEFLATE;
                    continue;
                }

                if (i == argc - 1) {
                    // all other flags require param
                    PARSE_FAIL("Expected parameter for flag '%s'", arg);
                }

                char *param = argv[i + 1];
                i += 1;

                if (strcmp(flag, FLAG_COMPRESSION_LONG) == 0) {
                    out_args->compression = param;
                } else if (strcmp(flag, FLAG_NAME_LONG) == 0) {
                    out_args->package_name = param;
                } else if (strcmp(flag, FLAG_MAPPINGS_LONG) == 0) {
                    out_args->mappings_path = param;
                } else if (strcmp(flag, FLAG_NAMESPACE_LONG) == 0) {
                    out_args->package_namespace = param;
                } else if (strcmp(flag, FLAG_OUTPUT_LONG) == 0) {
                    out_args->output_path = param;
                } else if (strcmp(flag, FLAG_PART_SIZE_LONG) == 0) {
                    size_t param_l = strtoull(param, NULL, 10);

                    if (errno != 0) {
                        PARSE_FAIL("Invalid param '%s' for flag '%s'", param, arg);
                    }

                    out_args->part_size = param_l;
                } else {
                    PARSE_FAIL("Unrecognized flag '%s'", arg);
                }

                continue;
            } else if (arg[0] == '-') {
                char *flag = arg + 1;

                if (i == argc - 1) {
                    // all short flags require param
                    PARSE_FAIL("Expected parameter for flag '%s'", arg);
                }

                char *param = argv[i + 1];
                i += 1;
                
                if (strcmp(flag, FLAG_COMPRESSION_SHORT) == 0) {
                    out_args->compression = param;
                } else if (strcmp(flag, FLAG_NAME_SHORT) == 0) {
                    out_args->package_name = param;
                } else if (strcmp(flag, FLAG_MAPPINGS_SHORT) == 0) {
                    out_args->mappings_path = param;
                } else if (strcmp(flag, FLAG_NAMESPACE_SHORT) == 0) {
                    out_args->package_namespace = param;
                } else if (strcmp(flag, FLAG_OUTPUT_SHORT) == 0) {
                    out_args->output_path = param;
                } else if (strcmp(flag, FLAG_PART_SIZE_SHORT) == 0) {
                    size_t param_l = strtoull(param, NULL, 10);

                    if (errno != 0) {
                        PARSE_FAIL("Invalid param '%s' for flag '%s'", param, arg);
                    }

                    out_args->part_size = param_l;
                } else {
                    PARSE_FAIL("Unrecognized flag '%s'", arg);
                }

                continue;
            }
        }

        switch (pos) {
            case POS_TARGET_PATH: {
                out_args->target_path = arg;
                break;
            }
            default: {
                PARSE_FAIL("Found unexpected positional arg '%s'", arg);
            }
        }

        pos += 1;
    }

    if (pos < REQUIRED_POS_ARGS) {
        PARSE_FAIL("Missing positional args (expected %u, found %lu)", REQUIRED_POS_ARGS, pos);
    }
    
    return NULL;
}
