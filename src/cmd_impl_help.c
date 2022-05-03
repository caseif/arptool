/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#include <stdio.h>
#include <string.h>

#include "arg_defs.h"
#include "arg_parse.h"
#include "cmd_impls.h"
#include "config.h"
#include "help.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define VERB_FORMAT "  %-12s %s\n"
#define PARAM_FORMAT "  %-*s %-*s %s\n"

#define GENERAL_USAGE "arptool <verb> <input path> [options]"
#define PACK_USAGE "arptool pack <input directory> [options]"
#define UNPACK_USAGE "arptool unpack <input package> [options]"
#define LIST_USAGE "arptool list <input package> [options]"

#define VERB_PACK "pack"
#define VERB_UNPACK "unpack"
#define VERB_LIST "list"

#define DESC_PACK "Creates an ARP archive from the directory at the given input path."
#define DESC_UNPACK "Extracts an ARP archive from the file at the given input path."
#define DESC_LIST "Lists the contents of the ARP archive at the given input path."

#define OPT_PACK_COMPRESS_SHORT "-c <type>"
#define OPT_PACK_COMPRESS_LONG "--compression=<type>"
#define OPT_PACK_COMPRESS_DESC "Compression type to use. Support options are `deflate` and `none`."

#define OPT_PACK_DEFLATE_SHORT ""
#define OPT_PACK_DEFLATE_LONG "--deflate"
#define OPT_PACK_DEFLATE_DESC "Use DEFLATE compression. Shorthand for `-c deflate`."

#define OPT_PACK_NAME_SHORT "-f <name>"
#define OPT_PACK_NAME_LONG "--name=<name>"
#define OPT_PACK_NAME_DESC "Name to use when generating package files."

#define OPT_PACK_MAPPINGS_SHORT "-m <path>"
#define OPT_PACK_MAPPINGS_LONG "--mappings=<path>"
#define OPT_PACK_MAPPINGS_DESC "Path to a CSV file providing supplemental media type mappings."

#define OPT_PACK_NAMESPACE_SHORT "-n <namespace>"
#define OPT_PACK_NAMESPACE_LONG "--namespace=<namespace>"
#define OPT_PACK_NAMESPACE_DESC "Namespace to use for the generated package."

#define OPT_PACK_OUTPUT_SHORT "-o <path>"
#define OPT_PACK_OUTPUT_LONG "--output=<path>"
#define OPT_PACK_OUTPUT_DESC "Path to the directory to output ARP archive files to."

#define OPT_PACK_PART_SHORT "-p <max size>"
#define OPT_PACK_PART_LONG "--part-size=<max size>"
#define OPT_PACK_PART_DESC "Maximum size in bytes for part files. The minimum supported value is 4096 bytes."

#define OPT_UNPACK_OUTPUT_SHORT "-o <path>"
#define OPT_UNPACK_OUTPUT_LONG "--output=<path>"
#define OPT_UNPACK_OUTPUT_DESC "Path to the directory to output extracted files to."

#define OPT_UNPACK_RESOURCE_SHORT "-r <path>"
#define OPT_UNPACK_RESOURCE_LONG "--resource=<path>"
#define OPT_UNPACK_RESOURCE_DESC "ARP path to a specific resource to extract from the current archive."

static const size_t opt_pack_max_short =
    MAX(sizeof(OPT_PACK_COMPRESS_SHORT),
    MAX(sizeof(OPT_PACK_DEFLATE_SHORT),
    MAX(sizeof(OPT_PACK_NAME_SHORT),
    MAX(sizeof(OPT_PACK_MAPPINGS_SHORT),
    MAX(sizeof(OPT_PACK_NAMESPACE_SHORT),
    MAX(sizeof(OPT_PACK_OUTPUT_SHORT),
        sizeof(OPT_PACK_PART_SHORT)))))));

static const size_t opt_pack_max_long =
    MAX(sizeof(OPT_PACK_COMPRESS_LONG),
    MAX(sizeof(OPT_PACK_DEFLATE_LONG),
    MAX(sizeof(OPT_PACK_NAME_LONG),
    MAX(sizeof(OPT_PACK_MAPPINGS_LONG),
    MAX(sizeof(OPT_PACK_NAMESPACE_LONG),
    MAX(sizeof(OPT_PACK_OUTPUT_LONG),
        sizeof(OPT_PACK_PART_LONG)))))));

static const size_t opt_unpack_max_short =
    MAX(sizeof(OPT_UNPACK_OUTPUT_SHORT),
        sizeof(OPT_UNPACK_RESOURCE_SHORT));

static const size_t opt_unpack_max_long =
    MAX(sizeof(OPT_UNPACK_OUTPUT_LONG),
        sizeof(OPT_UNPACK_RESOURCE_LONG));

static void _print_header(void) {
    printf("arptool version " PROJECT_VERSION "\n");
    printf("  Built with " COMPILER_ID " " COMPILER_VERSION " against libarp version " LIBARP_VERSION "\n");
    printf("\n");
}

static void _print_general_help(void) {
    printf("Usage: " GENERAL_USAGE "\n");
    printf("Available verbs:\n");
    printf(VERB_FORMAT, VERB_PACK, DESC_PACK);
    printf(VERB_FORMAT, VERB_UNPACK, DESC_UNPACK);
    printf(VERB_FORMAT, VERB_LIST, DESC_LIST);
    printf("For more help with a specific verb, use:\n");
    printf("  arptool --help <verb>\n");
}

static void _print_pack_help(void) {
    printf("Usage: " PACK_USAGE "\n");
    printf(DESC_PACK "\n");
    printf("Available options:\n");
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_COMPRESS_SHORT,
        (int) opt_pack_max_long, OPT_PACK_COMPRESS_LONG, OPT_PACK_COMPRESS_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_DEFLATE_SHORT,
        (int) opt_pack_max_long, OPT_PACK_DEFLATE_LONG, OPT_PACK_DEFLATE_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_NAME_SHORT,
        (int) opt_pack_max_long, OPT_PACK_NAME_LONG, OPT_PACK_NAME_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_MAPPINGS_SHORT,
        (int) opt_pack_max_long, OPT_PACK_MAPPINGS_LONG, OPT_PACK_MAPPINGS_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_NAMESPACE_SHORT,
        (int) opt_pack_max_long, OPT_PACK_NAMESPACE_LONG, OPT_PACK_NAMESPACE_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_OUTPUT_SHORT,
        (int) opt_pack_max_long, OPT_PACK_OUTPUT_LONG, OPT_PACK_OUTPUT_DESC);
    printf(PARAM_FORMAT, (int) opt_pack_max_short, OPT_PACK_PART_SHORT,
        (int) opt_pack_max_long, OPT_PACK_PART_LONG, OPT_PACK_PART_DESC);
}

static void _print_unpack_help(void) {
    printf("Usage: " UNPACK_USAGE "\n");
    printf(DESC_UNPACK "\n");
    printf("Available options:\n");
    printf(PARAM_FORMAT, (int) opt_unpack_max_short, OPT_UNPACK_OUTPUT_SHORT,
        (int) opt_unpack_max_long, OPT_UNPACK_OUTPUT_LONG, OPT_PACK_OUTPUT_DESC);
    printf(PARAM_FORMAT, (int) opt_unpack_max_short, OPT_UNPACK_RESOURCE_SHORT,
        (int) opt_unpack_max_long, OPT_UNPACK_RESOURCE_LONG, OPT_UNPACK_RESOURCE_DESC);
}

static void _print_list_help(void) {
    printf("Usage: " LIST_USAGE "\n");
    printf(DESC_LIST "\n");
}

int exec_cmd_help(arp_cmd_args_t *args) {
    _print_header();

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
        printf("For a list of available verbs, please run `arptool --help` without any additional parameters.\n");
    }

    return 0;
}

void print_general_usage_msg(void) {
    _print_general_help();
}
