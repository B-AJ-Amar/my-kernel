#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include <stddef.h>
// todo: add shell context
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
} cli_command_t;

typedef int (*cli_handler_t)(cli_command_t *command);

enum {
  CLI_SUCCESS,
  CLI_ERROR ,
  CLI_UNKNOWN_COMMAND ,
  CLI_ALREADY_REGISTERED,
};

int cli_register(const char *name, cli_handler_t handler);
int cli_execute(char *input);

bool cli_has_flag(const cli_command_t *command, char short_name);
bool cli_has_long_flag(const cli_command_t *command, const char *long_name);

#endif