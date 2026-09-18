#include <kernel/shell/cli.h>

#include <mm/heap/heap.h>
#include <string.h>

typedef struct cli_entry {
  char *name;
  cli_handler_t handler;
  struct cli_entry *next;
} cli_entry_t;

static cli_entry_t *cli_registry;
// todo: sort the commands for faster search, or use a hash table for O(1)
// lookup
static cli_entry_t *find_command(const char *name) {
  cli_entry_t *entry = cli_registry;

  while (entry != NULL) {
    if (strcmp(entry->name, name) == 0)
      return entry;

    entry = entry->next;
  }

  return NULL;
}

int cli_register(const char *name, cli_handler_t handler) {
  if (name == NULL || name[0] == '\0' || handler == NULL)
    return CLI_ERROR;

  if (find_command(name) != NULL)
    return CLI_ALREADY_REGISTERED;

  cli_entry_t *entry = kmalloc(sizeof(cli_entry_t));
  if (entry == NULL)
    return CLI_ERROR;

  entry->name = strdup(name);
  if (entry->name == NULL) {
    kfree(entry);
    return CLI_ERROR;
  }

  entry->handler = handler;
  entry->next = cli_registry;
  cli_registry = entry;
  return CLI_SUCCESS;
}

static char *next_token(char **input) {
  char *token = *input;

  while (*token == ' ' || *token == '\t')
    token++;

  if (*token == '\0') {
    *input = token;
    return NULL;
  }

  char *end = token;
  while (*end != '\0' && *end != ' ' && *end != '\t')
    end++;

  if (*end != '\0') {
    *end = '\0';
    *input = end + 1;
  } else {
    *input = end;
  }

  return token;
}

static size_t token_count(const char *input) {
  size_t count = 0;
  bool in_token = false;

  while (*input != '\0') {
    if (*input == ' ' || *input == '\t') {
      in_token = false;
    } else if (!in_token) {
      in_token = true;
      count++;
    }
    input++;
  }

  return count;
}

static bool is_long_flag(const char *token) {
  return token[0] == '-' && token[1] == '-' && token[2] != '\0';
}

static bool is_short_flag(const char *token) {
  return token[0] == '-' && token[1] != '\0' && token[1] != '-';
}

int cli_execute(char *input) {
  if (input == NULL)
    return CLI_ERROR;

  size_t max_items = token_count(input);
  if (max_items == 0)
    return CLI_ERROR;

  char *cursor = input;
  char *command_name = next_token(&cursor);
  cli_entry_t *entry = find_command(command_name);
  if (entry == NULL)
    return CLI_UNKNOWN_COMMAND;

  cli_command_t command = {
      .command = command_name,
      .flags = kmalloc(max_items * sizeof(cli_flag_t)),
      .flag_count = 0,
      .args = kmalloc(max_items * sizeof(char *)),
      .arg_count = 0,
  };

  if (command.flags == NULL || command.args == NULL) {
    kfree(command.flags);
    kfree(command.args);
    return CLI_ERROR;
  }

  bool parse_flags = true;
  char *token;
  while ((token = next_token(&cursor)) != NULL) {
    if (parse_flags && strcmp(token, "--") == 0) {
      parse_flags = false;
      continue;
    }

    if (parse_flags && is_long_flag(token)) {
      command.flags[command.flag_count].short_name = '\0';
      command.flags[command.flag_count].long_name = token + 2;
      command.flags[command.flag_count].value = true;
      command.flag_count++;
      continue;
    }

    if (parse_flags && is_short_flag(token)) {
      for (char *flag = token + 1; *flag != '\0'; flag++) {
        command.flags[command.flag_count].short_name = *flag;
        command.flags[command.flag_count].long_name = NULL;
        command.flags[command.flag_count].value = true;
        command.flag_count++;
      }
      continue;
    }

    command.args[command.arg_count++] = token;
  }

  int result = entry->handler(&command);
  kfree(command.flags);
  kfree(command.args);
  return result;
}

bool cli_has_flag(const cli_command_t *command, char short_name) {
  if (command == NULL)
    return false;

  for (size_t i = 0; i < command->flag_count; i++) {
    if (command->flags[i].value && command->flags[i].short_name == short_name)
      return true;
  }

  return false;
}

bool cli_has_long_flag(const cli_command_t *command, const char *long_name) {
  if (command == NULL || long_name == NULL)
    return false;

  for (size_t i = 0; i < command->flag_count; i++) {
    if (command->flags[i].value && command->flags[i].long_name != NULL &&
        strcmp(command->flags[i].long_name, long_name) == 0)
      return true;
  }

  return false;
}