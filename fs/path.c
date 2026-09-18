#include <fs/fs.h>
#include <mm/heap/heap.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

fs_path_t *fs_resolve_path(const char *path) {
  if (!path || path[0] != '/') {
    return NULL;
  }

  fs_path_t *resolved_path = (fs_path_t *)kmalloc(sizeof(fs_path_t));
  if (!resolved_path) {
    printf("Failed to allocate memory for resolved path\n");
    return NULL;
  }

  resolved_path->node = root_node;
  resolved_path->tail_node = root_node;
  resolved_path->path = strdup(path);

  char *token = strtok(resolved_path->path, "/");
  while (token) {
    fs_node_t *found_child = NULL;
    for (size_t i = 0; i < resolved_path->tail_node->children.count; i++) {
      if (strcmp(resolved_path->tail_node->children.items[i]->name, token) ==
          0) {
        found_child = resolved_path->tail_node->children.items[i];
        break;
      }
    }

    if (!found_child) {
      kfree(resolved_path->path);
      kfree(resolved_path);
      return NULL;
    }

    resolved_path->tail_node = found_child;

    token = strtok(NULL, "/");
  }

  return resolved_path;
}

bool check_duplicate_child(fs_node_t *parent, const char *name) {
  if (!parent || !name) {
    return false;
  }

  for (size_t i = 0; i < parent->children.count; i++) {
    if (strcmp(parent->children.items[i]->name, name) == 0) {
      return true;
    }
  }
  return false;
}