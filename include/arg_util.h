#pragma once

#include <stdbool.h>

#include "arg_parse.h"

char *get_output_path(const arp_cmd_args_t *args, bool *malloced);
