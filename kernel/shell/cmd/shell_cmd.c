#include <kernel/console.h>
#include <kernel/shell/cmd/cmd.h>
#include <kernel/shell/shell.h>
#include <stdio.h>
#include <string.h>

static bool parse_offset(const char *value, size_t *offset) {
  size_t result = 0;

  if (value == NULL || value[0] == '\0')
    return false;

  for (size_t i = 0; value[i] != '\0'; i++) {
    if (value[i] < '0' || value[i] > '9')
      return false;
    result = result * 10 + (size_t)(value[i] - '0');
  }

  *offset = result;
  return true;
}

int cmd_history(cli_context_t *context, cli_cmd_t *command) {
  (void)command;

  for (size_t i = 0; i < context->history_count; i++)
    printf("%u %s\n", (unsigned int)(i + 1), context->history[i]);
  return CLI_SUCCESS;
}

int cmd_help(cli_context_t *context, cli_cmd_t *command) {
  size_t offset = 0;

  (void)context;
  if (command->arg_count > 1 ||
      (command->arg_count == 1 &&
       !parse_offset(command->args[0], &offset)))
    return CLI_ERROR;

  cli_print_help(offset, shell_highlight_color);
  return CLI_SUCCESS;
}
