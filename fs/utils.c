#include <stdint.h>
#include <fs/fs.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <mm/heap/heap.h>   



bool fs_mkdir(char* path, const char *name) {
    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {
        printf("Failed to resolve path: %s\n", path);
        return false;
    }

    bool result = fs_create(resolved_path->tail_node, FS_DIR, name);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return result;
}

bool fs_touch(char* path, const char *name) {
    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {
        printf("Failed to resolve path: %s\n", path);
        return false;
    }

    bool result = fs_create(resolved_path->tail_node, FS_FILE, name);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return result;
}




bool rmdir(char* path, int allow_recursive) {

    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {  
        printf("Directory '%s' not found\n", path);
        return false;
    }
    if (resolved_path->tail_node->children.count > 0 && !allow_recursive) {
        printf("Directory '%s' is not empty. Use recursive delete to remove it.\n", path);
        kfree(resolved_path->path);
        kfree(resolved_path);
        return false;
    }
    bool is_deleted = fs_delete(resolved_path->tail_node);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return is_deleted;
}


bool fs_remove(char* path) {

    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {  
        printf("Node '%s' not found\n", path);
        return false;
    }
    bool is_deleted = fs_delete(resolved_path->tail_node);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return is_deleted;
}


char *fs_cat(char *path) {
    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {
        printf("Failed to resolve path: %s\n", path);
        return NULL;
    }

    fs_node_t *file_node = resolved_path->tail_node;
    if (file_node->type != FS_FILE) {
        printf("Path '%s' is not a file\n", path);
        kfree(resolved_path->path);
        kfree(resolved_path);
        return NULL;
    }

    char *content = fs_read(file_node, file_node->size);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return content;
}

fs_base_node_t *fs_ls(char *path) {
    fs_path_t *resolved_path = fs_resolve_path(path);
    if (!resolved_path) {
        printf("Failed to resolve path: %s\n", path);
        return NULL;
    }

    fs_node_t *dir_node = resolved_path->tail_node;
    if (dir_node->type != FS_DIR) {
        printf("Path '%s' is not a directory\n", path);
        kfree(resolved_path->path);
        kfree(resolved_path);
        return NULL;
    }

    fs_base_node_t *list = fs_list(dir_node);
    kfree(resolved_path->path);
    kfree(resolved_path);
    return list;
}