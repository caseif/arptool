/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include "arg_defs.h"
#include "arg_parse.h"
#include "compression_defines.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_10 10

#define ERR_MSG_BUF_LEN 256

#define CMP_LONG_FLAG(arg, len, flag) (strncmp(arg, flag, len) == 0 && strlen(flag) == (len))

static char *_parse_failed(const char *format, ...) {
    char *_cpp_err_msg_buf = malloc(ERR_MSG_BUF_LEN);
    size_t limit = ERR_MSG_BUF_LEN;

    va_list args;
    va_start(args, format);
    vsnprintf(_cpp_err_msg_buf, limit, format, args);
    va_end(args);

    return _cpp_err_msg_buf;
}

char *parse_args(int argc, char **argv, arp_cmd_args_t *out_args) {
    size_t pos = 0;
    bool stopped_args = false;

    out_args->is_help = false;

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

                char *eq_pos = strchr(arg, '=');

                size_t flag_len = 0;
                if (eq_pos != NULL) {
                    flag_len = eq_pos - flag;
                } else {
                    flag_len = strlen(flag);
                }

                if (CMP_LONG_FLAG(flag, flag_len, FLAG_HELP_LONG)) {
                    out_args->is_help = true;
                    continue;
                } else if (CMP_LONG_FLAG(flag, flag_len, NFLAG_DEFLATE)) {
                    if (eq_pos != NULL) {
                        return _parse_failed("Argument '%s' must not have a parameter", arg);
                    }

                    out_args->compression = CMPR_STR_DEFLATE;
                    continue;
                }

                if (eq_pos == NULL) {
                    return _parse_failed("Argument '%s' must have a parameter", arg);
                }

                if (i == argc - 1) {
                    // all other flags require param
                    return _parse_failed("Expected parameter for flag '%s'", arg);
                }

                char *param = eq_pos + 1;

                if (CMP_LONG_FLAG(flag, flag_len, FLAG_COMPRESSION_LONG)) {
                    out_args->compression = param;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_NAME_LONG)) {
                    out_args->package_name = param;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_MAPPINGS_LONG)) {
                    out_args->mappings_path = param;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_NAMESPACE_LONG)) {
                    out_args->package_namespace = param;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_OUTPUT_LONG)) {
                    out_args->output_path = param;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_PART_SIZE_LONG)) {
                    uint64_t param_l = strtoull(param, NULL, BASE_10);

                    if (errno != 0) {
                        return _parse_failed("Invalid param '%s' for flag '%s'", param, arg);
                    }

                    out_args->part_size = param_l;
                } else if (CMP_LONG_FLAG(flag, flag_len, FLAG_RESOURCE_PATH_LONG)) {
                    out_args->resource_path = param;
                } else {
                    return _parse_failed("Unrecognized flag '%s'", arg);
                }

                continue;
            } else if (arg[0] == '-') {
                char *flag = arg + 1;

                if (strcmp(flag, FLAG_HELP_SHORT) == 0 || strcmp(flag, FLAG_HELP_SHORT_ALT) == 0) {
                    out_args->is_help = true;
                    continue;
                }

                if (i == argc - 1) {
                    // all short flags require param
                    return _parse_failed("Expected parameter for flag '%s'", arg);
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
                    uint64_t param_l = strtoull(param, NULL, BASE_10);

                    if (errno != 0) {
                        return _parse_failed("Invalid param '%s' for flag '%s'", param, arg);
                    }

                    out_args->part_size = param_l;
                } else if (strcmp(flag, FLAG_RESOURCE_PATH_SHORT) == 0) {
                    out_args->resource_path = param;
                } else {
                    return _parse_failed("Unrecognized flag '%s'", arg);
                }

                continue;
            }
        }

        switch (pos) {
            case POS_VERB: {
                out_args->verb = arg;
                break;
            }
            case POS_SRC_PATH: {
                out_args->src_path = arg;
                break;
            }
            default: {
                return _parse_failed("Found unexpected positional arg '%s'", arg);
            }
        }

        pos += 1;
    }

    if (!out_args->is_help && pos < REQUIRED_POS_ARGS) {
        return _parse_failed("Missing positional args (expected %u, found %lu)", REQUIRED_POS_ARGS, pos);
    }
    
    return NULL;
}
