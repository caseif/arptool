#pragma once

#include "arg_parse.h"

#include "stdio.h"

enum LogLevel {
    LogLevelInfo,
    LogLevelError
};

void arptool_print(const arp_cmd_args_t *cmd_args, enum LogLevel level, const char *fmt, ...);
