#include <fs/fs.h>
#include <kernel/bins/editor.h>
#include <kernel/console.h>
#include <kernel/shell/cmd/cmd.h>
#include <kernel/shell/shell.h>
#include <kernel/shell/utils.h>
#include <mm/heap/heap.h>
#include <stdio.h>
#include <string.h>

static bool resolve_parent(const cli_context_t *context, const char *target,
                           fs_path_t **parent, char *name, size_t name_size) {
  char path[CLI_PATH_SIZE];
  char *slash;

  if (!shell_build_path(context, target, path, sizeof(path)))
    return false;

  slash = path + strlen(path) - 1;
  while (slash > path && *slash != '/')
    slash--;

  if (*slash != '/' || slash[1] == '\0' || strlen(slash + 1) >= name_size)
    return false;

  snprintf(name, name_size, "%s", slash + 1);
  if (slash == path)
    path[1] = '\0';
  else
    *slash = '\0';

  *parent = fs_resolve_path(path);
  if (*parent == NULL)
    return false;
  if ((*parent)->tail_node->type != FS_DIR) {
    kfree((*parent)->path);
    kfree(*parent);
    *parent = NULL;
    return false;
  }
  return true;
}

static fs_path_t *resolve_target(const cli_context_t *context,
                                 const char *target) {
  char path[CLI_PATH_SIZE];

  if (!shell_build_path(context, target, path, sizeof(path)))
    return NULL;
  return fs_resolve_path(path);
}

static size_t ls_entry_width(const fs_node_t *node) {
  return strlen((char *)node->name) + (node->type == FS_DIR ? 1 : 0);
}

static void ls_print_entry(const fs_node_t *node) {
  if (node->type == FS_DIR)
    printf("%s%s/\033[15,0]", shell_highlight_color, node->name);
  else
    printf("%s", node->name);
}

int cmd_pwd(cli_context_t *context, cli_cmd_t *command) {
  (void)command;
  printf("%s\n", context->cwd);
  return CLI_SUCCESS;
}

int cmd_cd(cli_context_t *context, cli_cmd_t *command) {
  char path[CLI_PATH_SIZE];

  if (command->arg_count > 1) {
    printf("cd: too many arguments\n");
    return CLI_ERROR;
  }

  const char *target = command->arg_count == 0 ? "/" : command->args[0];
  if (!shell_build_path(context, target, path, sizeof(path))) {
    printf("cd: path too long\n");
    return CLI_ERROR;
  }

  fs_path_t *resolved = fs_resolve_path(path);
  if (resolved == NULL || resolved->tail_node->type != FS_DIR) {
    printf("cd: no such directory: %s\n", target);
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
    return CLI_ERROR;
  }

  snprintf(context->cwd, sizeof(context->cwd), "%s", path);
  kfree(resolved->path);
  kfree(resolved);
  return CLI_SUCCESS;
}

int cmd_ls(cli_context_t *context, cli_cmd_t *command) {
  char path[CLI_PATH_SIZE];
  const char *target = command->arg_count == 0 ? "." : command->args[0];

  if (command->arg_count > 1) {
    printf("ls: too many arguments\n");
    return CLI_ERROR;
  }

  if (!shell_build_path(context, target, path, sizeof(path))) {
    printf("ls: path too long\n");
    return CLI_ERROR;
  }

  fs_path_t *resolved = fs_resolve_path(path);
  if (resolved == NULL || resolved->tail_node->type != FS_DIR) {
    printf("ls: no such directory: %s\n", target);
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
    return CLI_ERROR;
  }

  fs_node_t *directory = resolved->tail_node;
  size_t max_width = 0;
  for (size_t i = 0; i < directory->children.count; i++) {
    size_t width = ls_entry_width(directory->children.items[i]);
    if (width > max_width)
      max_width = width;
  }

  size_t columns = max_width <= 24 ? 3 : (max_width <= 38 ? 2 : 1);
  size_t column_width = columns == 1 ? max_width : 80 / columns;
  size_t rows = (directory->children.count + columns - 1) / columns;

  for (size_t row = 0; row < rows; row++) {
    for (size_t column = 0; column < columns; column++) {
      size_t index = row + column * rows;
      if (index >= directory->children.count)
        continue;

      fs_node_t *child = directory->children.items[index];
      size_t width = ls_entry_width(child);
      ls_print_entry(child);

      if (column + 1 < columns && index + rows < directory->children.count) {
        for (size_t spaces = width; spaces < column_width; spaces++)
          putchar(' ');
      }
    }
    putchar('\n');
  }

  kfree(resolved->path);
  kfree(resolved);
  return CLI_SUCCESS;
}

int cmd_clear(cli_context_t *context, cli_cmd_t *command) {
  (void)context;
  (void)command;
  console_clear();
  return CLI_SUCCESS;
}

int cmd_touch(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *parent;
  char name[MAX_NAME_LENGTH];

  if (command->arg_count != 1 ||
      !resolve_parent(context, command->args[0], &parent, name, sizeof(name)))
    return CLI_ERROR;

  bool result = fs_create(parent->tail_node, FS_FILE, name);
  kfree(parent->path);
  kfree(parent);
  return result ? CLI_SUCCESS : CLI_ERROR;
}

int cmd_mkdir(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *parent;
  char name[MAX_NAME_LENGTH];

  if (command->arg_count != 1 ||
      !resolve_parent(context, command->args[0], &parent, name, sizeof(name)))
    return CLI_ERROR;

  bool result = fs_create(parent->tail_node, FS_DIR, name);
  kfree(parent->path);
  kfree(parent);
  return result ? CLI_SUCCESS : CLI_ERROR;
}

static int delete_command(cli_context_t *context, cli_cmd_t *command,
                          fs_node_type_t type) {
  fs_path_t *resolved;

  if (command->arg_count != 1)
    return CLI_ERROR;

  resolved = resolve_target(context, command->args[0]);
  if (resolved == NULL || resolved->tail_node->type != type) {
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
    return CLI_ERROR;
  }

  bool result = fs_delete(resolved->tail_node);
  kfree(resolved->path);
  kfree(resolved);
  return result ? CLI_SUCCESS : CLI_ERROR;
}

int cmd_rm(cli_context_t *context, cli_cmd_t *command) {
  return delete_command(context, command, FS_FILE);
}

int cmd_rmdir(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *resolved;

  if (command->arg_count == 1) {
    resolved = resolve_target(context, command->args[0]);
    if (resolved != NULL && resolved->tail_node->type == FS_DIR &&
        resolved->tail_node->children.count != 0) {
      kfree(resolved->path);
      kfree(resolved);
      return CLI_ERROR;
    }
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
  }
  return delete_command(context, command, FS_DIR);
}

int cmd_cat(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *resolved;
  char *content;

  if (command->arg_count != 1)
    return CLI_ERROR;

  resolved = resolve_target(context, command->args[0]);
  if (resolved == NULL || resolved->tail_node->type != FS_FILE) {
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
    return CLI_ERROR;
  }

  content = fs_read(resolved->tail_node, resolved->tail_node->size);
  if (content != NULL) {
    printf("%s\n", content);
    kfree(content);
  }
  kfree(resolved->path);
  kfree(resolved);
  return content != NULL ? CLI_SUCCESS : CLI_ERROR;
}

int cmd_write(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *resolved;
  fs_path_t *parent;
  char name[MAX_NAME_LENGTH];
  size_t length;
  bool result;

  if (command->arg_count != 2)
    return CLI_ERROR;

  resolved = resolve_target(context, command->args[0]);
  if (resolved != NULL) {
    if (resolved->tail_node->type != FS_FILE) {
      kfree(resolved->path);
      kfree(resolved);
      return CLI_ERROR;
    }
  } else {
    if (!resolve_parent(context, command->args[0], &parent, name, sizeof(name)))
      return CLI_ERROR;
    if (!fs_create(parent->tail_node, FS_FILE, name)) {
      kfree(parent->path);
      kfree(parent);
      return CLI_ERROR;
    }
    kfree(parent->path);
    kfree(parent);
    resolved = resolve_target(context, command->args[0]);
    if (resolved == NULL)
      return CLI_ERROR;
  }

  length = strlen(command->args[1]);
  result =
      fs_write(resolved->tail_node, (const uint8_t *)command->args[1], length);
  kfree(resolved->path);
  kfree(resolved);
  return result ? CLI_SUCCESS : CLI_ERROR;
}

int cmd_edit(cli_context_t *context, cli_cmd_t *command) {
  fs_path_t *resolved;
  fs_path_t *parent;
  char name[MAX_NAME_LENGTH];
  int result;

  if (command->arg_count != 1)
    return CLI_ERROR;

  resolved = resolve_target(context, command->args[0]);
  if (resolved != NULL && resolved->tail_node->type != FS_FILE) {
    kfree(resolved->path);
    kfree(resolved);
    return CLI_ERROR;
  }

  if (resolved == NULL) {
    if (!resolve_parent(context, command->args[0], &parent, name, sizeof(name)))
      return CLI_ERROR;
    if (!fs_create(parent->tail_node, FS_FILE, name)) {
      kfree(parent->path);
      kfree(parent);
      return CLI_ERROR;
    }
    kfree(parent->path);
    kfree(parent);
    resolved = resolve_target(context, command->args[0]);
  }

  if (resolved == NULL) {
    return CLI_ERROR;
  }

  if (resolved->tail_node->type != FS_FILE) {
    if (resolved != NULL) {
      kfree(resolved->path);
      kfree(resolved);
    }
    return CLI_ERROR;
  }

  result = editor_run(resolved->tail_node);
  kfree(resolved->path);
  kfree(resolved);
  return result == 0 ? CLI_SUCCESS : CLI_ERROR;
}