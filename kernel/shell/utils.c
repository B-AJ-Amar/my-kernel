#include <kernel/shell/utils.h>

#include <stdio.h>
#include <string.h>

static bool normalize_path(char *path, size_t buffer_size) {
  char source[CLI_PATH_SIZE];
  size_t segment_starts[CLI_PATH_SIZE / 2];
  size_t segment_count = 0;
  size_t read = 1;
  size_t write = 1;

  if (path[0] != '/' || strlen(path) >= sizeof(source))
    return false;

  memcpy(source, path, strlen(path) + 1);

  while (source[read] != '\0') {
    size_t start;
    size_t length;

    while (source[read] == '/')
      read++;
    start = read;
    while (source[read] != '\0' && source[read] != '/')
      read++;
    length = read - start;

    if (length == 0 || (length == 1 && source[start] == '.'))
      continue;

    if (length == 2 && source[start] == '.' && source[start + 1] == '.') {
      if (segment_count > 0) {
        write = segment_starts[--segment_count];
        path[write] = '\0';
      }
      continue;
    }

    if (segment_count >= sizeof(segment_starts) / sizeof(segment_starts[0]) ||
        write + length + (write > 1 ? 1 : 0) >= buffer_size)
      return false;

    if (write > 1)
      path[write++] = '/';
    segment_starts[segment_count++] = write;
    memcpy(&path[write], &source[start], length);
    write += length;
    path[write] = '\0';
  }

  path[write] = '\0';
  return true;
}

bool shell_build_path(const cli_context_t *context, const char *path,
                      char *buffer, size_t buffer_size) {
  int written;

  if (context == NULL || path == NULL || buffer == NULL || buffer_size == 0)
    return false;

  if (path[0] == '/')
    written = snprintf(buffer, buffer_size, "%s", path);
  else if (strcmp(context->cwd, "/") == 0)
    written = snprintf(buffer, buffer_size, "/%s", path);
  else
    written = snprintf(buffer, buffer_size, "%s/%s", context->cwd, path);

  return written >= 0 && (size_t)written < buffer_size &&
         normalize_path(buffer, buffer_size);
}