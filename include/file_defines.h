/*
 * This file is a part of libarp.
 * Copyright (c) 2020-2022, Max Roncace <mproncace@protonmail.com>
 *
 * This software is made available under the MIT license. You should have
 * received a copy of the full license text with this software. If not, the
 * license text may be accessed at https://opensource.org/licenses/MIT.
 */

#pragma once

#define WIN32_PATH_DELIM '\\'
#define UNIX_PATH_DELIM '/'

#ifdef _WIN32
#define PATH_DELIM WIN32_PATH_DELIM
#define IS_PATH_DELIM(c) ((c) == UNIX_PATH_DELIM || (c) == WIN32_PATH_DELIM)
#else
#define PATH_DELIM UNIX_PATH_DELIM
#define IS_PATH_DELIM(c) ((c) == UNIX_PATH_DELIM)
#endif


#define EXTENSION_DELIM '.'

extern int make_iso_compilers_happy;
