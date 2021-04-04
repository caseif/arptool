#include "arg_parse.h"
#include "cmd_impls.h"

#include "libarp/unpack.h"

#include <stdio.h>
#include <string.h>

#define MAX(a, b) ((a > b) ? (a) : (b))

void exec_cmd_list(arp_cmd_args_t *args) {
    char *src_path = args->src_path;

    ArgusPackage package;
    if (load_package_from_file(src_path, &package) != 0) {
        printf("Failed to load package (libarp says: %s)\n", libarp_get_error());
        return;
    }

    arp_resource_info_t *res_info;
    size_t res_count;
    if (list_resources(package, &res_info, &res_count) != 0) {
        printf("Failed to list resources in package (libarp says: %s)\n", libarp_get_error());
        return;
    }

    size_t max_path = strlen("PATH");
    size_t max_ext = 0;
    size_t max_mt = strlen("TYPE");
    for (size_t i = 0; i < res_count; i++) {
        arp_resource_info_t *res = &res_info[i];
        max_path = MAX(max_path, strlen(res->path));
        max_ext = MAX(max_ext, strlen(res->extension));
        max_mt = MAX(max_mt, strlen(res->media_type));
    }

    printf("TYPE");

    for (size_t i = 0; i < max_mt - strlen("TYPE"); i++) {
        printf(" ");
    }

    printf("   ");

    printf("PATH\n");

    for (size_t i = 0; i < max_mt + max_path + 3; i++) {
        printf("-");
    }
    printf("\n");

    for (size_t i = 0; i < res_count; i++) {
        arp_resource_info_t *res = &res_info[i];

        printf(res->media_type);

        size_t type_padding = max_mt - strlen(res->media_type);
        for (size_t i = 0; i < type_padding; i++) {
            printf(" ");
        }

        printf("   ");

        printf(res->path);

        printf("\n");
    }
}
