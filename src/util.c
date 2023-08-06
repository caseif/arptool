#include "util.h"

#include <stdarg.h>

void arptool_print(const arp_cmd_args_t *cmd_args, enum LogLevel level, const char *fmt, ...) {

    if (cmd_args->verbosity == VerbositySilent) {
        return;
    } else if (cmd_args->verbosity == VerbosityQuiet && level == LogLevelInfo) {
        return;
    }

    FILE *out_stream = level == LogLevelError ? stderr : stdout;

    va_list args;
    va_start(args, fmt);
    vfprintf(out_stream, fmt, args);
    va_end(args);
}
