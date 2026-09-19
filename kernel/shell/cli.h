#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include <stddef.h>

#define CLI_PATH_SIZE 256

typedef struct {
  char cwd[CLI_PATH_SIZE];
  char **history;
  size_t history_count;
  size_t history_capacity;
} cli_context_t;

typedef struct {
  char short_name;
  const char *long_name;
  bool value;
} cli_flag_t;

typedef struct {
  const char *command;

  cli_flag_t *flags;
  size_t flag_count;

  char **args;
  size_t arg_count;
} cli_cmd_t;

typedef int (*cli_handler_t)(cli_context_t *context, cli_cmd_t *command);

enum {
  CLI_SUCCESS,
  CLI_ERROR,
  CLI_UNKNOWN_COMMAND,
  CLI_ALREADY_REGISTERED,
};

int cli_register(const char *name, const char *description,
                 cli_handler_t handler);
void cli_init(cli_context_t *context);
int cli_execute(cli_context_t *context, char *input);
void cli_print_help(size_t offset, const char *highlight_color);

bool cli_has_flag(const cli_cmd_t *command, char short_name);
bool cli_has_long_flag(const cli_cmd_t *command, const char *long_name);

#endif