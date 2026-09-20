#ifndef SHELL_UTILS_H
#define SHELL_UTILS_H

#include <kernel/shell/cli.h>
#include <stdbool.h>
#include <stddef.h>

bool shell_build_path(const cli_context_t *context, const char *path,
                      char *buffer, size_t buffer_size);

#endif