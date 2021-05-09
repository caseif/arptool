#include <stdio.h>
#include <string.h>

#include "arg_defs.h"
#include "arg_parse.h"
#include "cmd_impls.h"

static void _print_general_help(void) {
    printf("General help\n");
}

static void _print_pack_help(void) {
    printf("Pack help\n");
}

static void _print_unpack_help(void) {
    printf("Unpack help\n");
}

static void _print_list_help(void) {
    printf("List help\n");
}

int exec_cmd_help(arp_cmd_args_t *args) {
    if (args->verb == NULL) {
        _print_general_help();
    } else if (strcmp(args->verb, VERB_PACK) == 0) {
        _print_pack_help();
    } else if (strcmp(args->verb, VERB_UNPACK) == 0) {
        _print_unpack_help();
    } else if (strcmp(args->verb, VERB_LIST) == 0) {
        _print_list_help();
    } else {
        printf("Unrecognized verb %s\n", args->verb);
    }

    return 0;
}
