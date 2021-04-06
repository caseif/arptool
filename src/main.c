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
#include "cmd_impls.h"

#include "libarp/defines.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sigtrap_handler(int signum) {
}

int main(int argc, char **argv) {
    signal(SIGTRAP, sigtrap_handler);

    arp_cmd_args_t args;
    memset(&args, 0, sizeof(args));

    char *parse_err = parse_args(argc, argv, &args);

    if (parse_err != NULL) {
        printf("%s\n", parse_err);

        free(parse_err);

        exit(-1);
    }

    if (strcmp(args.verb, VERB_PACK) != 0) {
        if (args.compression != NULL) {
            printf("Compression param does not make sense with specified verb\n");
            exit(-1);
        }
        if (args.mappings_path != NULL) {
            printf("Mappings path param does not make sense with specified verb\n");
            exit(-1);
        }
        if (args.package_name != NULL) {
            printf("Package name param does not make sense with specified verb\n");
            exit(-1);
        }
        if (args.package_namespace != NULL) {
            printf("Namespace param does not make sense with specified verb\n");
            exit(-1);
        }
        if (args.part_size != 0) {
            printf("Part size param does not make sense with specified verb\n");
            exit(-1);
        }
    }

    if (args.package_namespace != NULL && strlen(args.package_namespace) > ARP_NAMESPACE_MAX) {
        printf("Namespace is too long (max %d chars)\n", ARP_NAMESPACE_MAX);
        return EINVAL;
    }

    if (strcmp(args.verb, VERB_HELP) == 0) {
        printf("Help coming soon(TM).\n");
        return 0;
    } else if (strcmp(args.verb, VERB_PACK) == 0) {
        return exec_cmd_pack(&args);
    } else if (strcmp(args.verb, VERB_UNPACK) == 0) {
        return exec_cmd_unpack(&args);
    } else if (strcmp(args.verb, VERB_LIST) == 0) {
        return exec_cmd_list(&args);
    } else {
        printf("Unrecognized verb: %s\n", args.verb);
        return EINVAL;
    }
}
