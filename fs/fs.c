#include <fs/fs.h>
#include <mm/heap/heap.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

fs_node_t *root_node = NULL;
static bool recursive_delete(fs_node_t *node);

void fs_init() {
  root_node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
  if (!root_node) {
    printf("Failed to allocate memory for root node\n");
    return;
  }

  printf("Initializing filesystem... ptr: 0x%x\n", root_node);

  root_node->type = FS_DIR;
  root_node->parent = NULL;
  root_node->children.items = NULL;
  root_node->children.count = 0;
  root_node->children.capacity = 0;
  root_node->data = NULL;
  root_node->size = 0;
  root_node->capacity = 0;

  snprintf(root_node->name, sizeof(root_node->name), "/");
  fs_create(root_node, FS_DIR, "root");
  fs_create(root_node, FS_DIR, "home");
  fs_create(root_node, FS_DIR, "bin");
  fs_create(root_node, FS_DIR, "etc");
  fs_create(root_node, FS_DIR, "usr");
  fs_create(root_node, FS_DIR, "var");
  fs_create(root_node, FS_DIR, "tmp");
}

int validate_filename(const char *name) {
  if (!name || strlen(name) == 0 || strlen(name) >= MAX_NAME_LENGTH) {
    return -1;
  }

  for (size_t i = 0; i < strlen(name); i++) {
    char c = name[i];
    if (!(isalnum(c) || c == '_' || c == '-' || c == '.')) {
      return -1;
    }
  }

  return 0;
}

bool fs_create(fs_node_t *parent, fs_node_type_t type, const char *name) {
  if (!parent || !name || validate_filename(name) != 0) {
    return false;
  }

  if (check_duplicate_child(parent, name)) {
    printf(
        "A child with the name '%s' already exists in the parent directory\n",
        name);
    return false;
  }

  fs_node_t *new_node = (fs_node_t *)kmalloc(sizeof(fs_node_t));
  if (!new_node) {
    printf("Failed to allocate memory for new node\n");
    return false;
  }

  snprintf(new_node->name, sizeof(new_node->name), "%s", name);
  new_node->type = type;
  new_node->parent = parent;
  new_node->children.items = NULL;
  new_node->children.count = 0;
  new_node->children.capacity = 0;
  new_node->data = NULL;
  new_node->size = 0;
  new_node->capacity = 0;

  if (parent->children.count == parent->children.capacity) {
    size_t new_capacity =
        (parent->children.capacity == 0) ? 2 : parent->children.capacity * 2;
    fs_node_t **new_items = (fs_node_t **)krealloc(
        parent->children.items, new_capacity * sizeof(fs_node_t *));
    if (!new_items) {
      printf("Failed to allocate memory for children list\n");
      kfree(new_node);
      return false;
    }
    parent->children.items = new_items;
    parent->children.capacity = new_capacity;
  }

  parent->children.items[parent->children.count++] = new_node;
  return true;
}

static bool recursive_delete(fs_node_t *node) {
  if (!node) {
    return false;
  }

  if (node->type == FS_DIR) {
    while (node->children.count > 0) {
      recursive_delete(node->children.items[0]);
    }
    kfree(node->children.items);
  }

  kfree(node->data);
  kfree(node);
  return true;
}

bool fs_delete(fs_node_t *node) {
  if (!node) {
    return false;
  }

  if (node->parent) {
    fs_node_t *parent = node->parent;
    size_t index = 0;
    while (index < parent->children.count &&
           parent->children.items[index] != node) {
      index++;
    }

    if (index < parent->children.count) {
      for (size_t i = index; i < parent->children.count - 1; i++) {
        parent->children.items[i] = parent->children.items[i + 1];
      }
      parent->children.count--;
    }
  }

  if (node->type == FS_FILE) {
    kfree(node->data);
    kfree(node);
    return true;
  }

  return recursive_delete(node);
}

char *fs_read(fs_node_t *node, size_t size) {
  if (!node || node->type != FS_FILE) {
    printf("Invalid node or node is not a file\n");
    return false;
  }

  if (size > node->size) {
    size = node->size;
  }

  char *buffer = (char *)kmalloc(size + 1);
  if (!buffer) {
    printf("Failed to allocate memory for file data\n");
    return false;
  }

  memcpy(buffer, node->data, size);
  buffer[size] = '\0';
  return buffer;
}

bool fs_write(fs_node_t *node, const uint8_t *data, size_t size) {
  if (!node || node->type != FS_FILE || !data) {
    return false;
  }

  if (size > node->capacity) {
    uint8_t *new_data = (uint8_t *)krealloc(node->data, size);
    if (!new_data) {
      printf("Failed to allocate memory for file data\n");
      return false;
    }
    node->data = new_data;
    node->capacity = size;
  }
  memcpy(node->data, data, size);
  node->size = size;
  return true;
}

uintptr_t fs_open_file(char *path) {
  fs_path_t *resolved_path = fs_resolve_path(path);
  if (!resolved_path) {
    printf("Failed to resolve path: %s\n", path);
    return (uintptr_t)NULL;
  }
  return (uintptr_t)resolved_path->tail_node;
}

fs_base_node_t *fs_list(fs_node_t *parent) {
  if (!parent || parent->type != FS_DIR) {
    return NULL;
  }

  fs_base_node_t *list = (fs_base_node_t *)kmalloc(
      (parent->children.count + 1) * sizeof(fs_base_node_t));
  if (!list) {
    printf("Failed to allocate memory for directory listing\n");
    return NULL;
  }

  for (size_t i = 0; i < parent->children.count; i++) {
    list[i].type = parent->children.items[i]->type;
    snprintf(list[i].name, sizeof(list[i].name), "%s",
             parent->children.items[i]->name);
  }
  list[parent->children.count] = (fs_base_node_t){0};
  return list;
}