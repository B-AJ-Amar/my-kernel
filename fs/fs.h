#ifndef FS_FS_H
#define FS_FS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define MAX_NAME_LENGTH 32


typedef enum {
    FS_FILE,
    FS_DIR
} fs_node_type_t;

typedef struct fs_node_list_t {
    struct fs_node_t **items;
    size_t count;
    size_t capacity;
} fs_node_list_t;

typedef struct fs_base_node_t {
    char name[MAX_NAME_LENGTH];
    fs_node_type_t type;
} fs_base_node_t;

typedef struct fs_node_t {
    char name[MAX_NAME_LENGTH];
    fs_node_type_t type;

    struct fs_node_t *parent;

    struct fs_node_list_t children;

    uint8_t *data;
    size_t size;
    size_t capacity;
} fs_node_t;

typedef struct {
    fs_node_t *node;
    fs_node_t *tail_node;
    char *path;
} fs_path_t;

extern fs_node_t *root_node;
void fs_init();
bool fs_create(fs_node_t *parent, fs_node_type_t type, const char *name);
bool fs_move(fs_node_t *node, fs_node_t *new_parent);
bool fs_rename(fs_node_t *node, const char *new_name);
bool fs_write(fs_node_t *node, const uint8_t *data, size_t size);
bool fs_delete(fs_node_t *node);
char* fs_read(fs_node_t *node, size_t size);


bool fs_mkdir(char* path, const char *name);
bool fs_touch(char* path, const char *name);
bool fs_rmdir(char* path, int allow_recursive);
bool fs_remove(char* path);
char* fs_cat(char *path);
fs_base_node_t *fs_ls(char *path);

uintptr_t fs_open_file(char *path);
fs_base_node_t *fs_list(fs_node_t *parent);
fs_path_t *fs_resolve_path(const char *path);
bool check_duplicate_child(fs_node_t *parent, const char *name);

#endif // FS_FS_H