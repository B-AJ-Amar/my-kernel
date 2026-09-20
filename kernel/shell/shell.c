#include <kernel/shell/cli.h>
#include <kernel/shell/cmd/cmd.h>
#include <kernel/tty.h>
#include <mm/heap/heap.h>
#include <stdio.h>
#include <string.h>
#define blue "\033[9,0]"
#define green "\033[2,0]"
#define white "\033[15,0]"
static cli_context_t shell_context;
const char *shell_highlight_color = green;

void shell_init(void) {
  cli_init(&shell_context);
  cli_register("pwd", "print the current directory", cmd_pwd);
  cli_register("go", "change the current directory", cmd_cd);
  cli_register("ls", "list directory contents", cmd_ls);
  cli_register("clear", "clear the console", cmd_clear);
  cli_register("nwfile", "create an empty file", cmd_touch);
  cli_register("nwdir", "create a directory", cmd_mkdir);
  cli_register("del", "remove a file", cmd_rm);
  cli_register("rmdir", "remove an empty directory", cmd_rmdir);
  cli_register("read", "read file", cmd_cat);
  cli_register("write", "write quoted content to a file", cmd_write);
  cli_register("edit", "open the text editor", cmd_edit);
  cli_register("print", "print text to the console", cmd_print);
  cli_register("info", "show CPU information", cmd_info);
  cli_register("tree", "show a directory tree", cmd_tree);
  cli_register("uptime", "show system uptime", cmd_uptime);
  cli_register("rename", "rename a file or directory", cmd_rename);
  cli_register("move", "move a file or directory", cmd_move);
  cli_register("history", "show command history", cmd_history);
  cli_register("help", "show available commands", cmd_help);
}

static bool shell_add_history(const char *input) {
  char **new_history;

  if (input == NULL || input[0] == '\0')
    return true;

  if (shell_context.history_count == shell_context.history_capacity) {
    size_t capacity = shell_context.history_capacity == 0
                          ? 16
                          : shell_context.history_capacity * 2;
    new_history = krealloc(shell_context.history, capacity * sizeof(char *));
    if (new_history == NULL)
      return false;
    shell_context.history = new_history;
    shell_context.history_capacity = capacity;
  }

  shell_context.history[shell_context.history_count] = strdup(input);
  if (shell_context.history[shell_context.history_count] == NULL)
    return false;
  shell_context.history_count++;
  return true;
}

void shell_run(tty_t *tty) {
  char input[TTY_BUFFER_SIZE];
  printf("\033[15,0]Welcome to the shell ... Type 'help' for a list of "
         "commands.\n");
  while (1) {
    printf("%s[ %s%s%s ] >\033[15,0] ", shell_highlight_color, white,
           shell_context.cwd, shell_highlight_color);
    tty_readline(tty, input, sizeof(input));
    shell_add_history(input);

    int result = cli_execute(&shell_context, input);
    if (result == CLI_UNKNOWN_COMMAND)
      printf("unknown command\n");
    else if (result == CLI_ERROR)
      printf("command error\n");
  }
}
