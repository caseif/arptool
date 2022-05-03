/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#pragma once

#include "arg_parse.h"

int exec_cmd_pack(arp_cmd_args_t *args);

int exec_cmd_unpack(arp_cmd_args_t *args);

int exec_cmd_list(arp_cmd_args_t *args);

int exec_cmd_help(arp_cmd_args_t *args);
