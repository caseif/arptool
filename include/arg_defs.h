/*
 * This file is a part of libarp.
 * Copyright (c) 2020, Max Roncace <mproncace@gmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#pragma once

#define FLAG_HELP_SHORT "?"
#define FLAG_HELP_SHORT_ALT "h"
#define FLAG_HELP_LONG "help"
#define FLAG_COMPRESSION_SHORT "c"
#define FLAG_COMPRESSION_LONG "compression"
#define FLAG_NAME_SHORT "f"
#define FLAG_NAME_LONG "name"
#define FLAG_MAPPINGS_SHORT "m"
#define FLAG_MAPPINGS_LONG "mappings"
#define FLAG_NAMESPACE_SHORT "n"
#define FLAG_NAMESPACE_LONG "namespace"
#define FLAG_OUTPUT_SHORT "o"
#define FLAG_OUTPUT_LONG "output"
#define FLAG_PART_SIZE_SHORT "p"
#define FLAG_PART_SIZE_LONG "part-size"
#define FLAG_RESOURCE_PATH_SHORT "r"
#define FLAG_RESOURCE_PATH_LONG "resource"

#define NFLAG_DEFLATE "deflate"

#define POS_VERB 0
#define POS_SRC_PATH 1

#define REQUIRED_POS_ARGS 2

#define VERB_PACK "pack"
#define VERB_UNPACK "unpack"
#define VERB_LIST "list"
