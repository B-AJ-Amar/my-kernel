#ifndef SHELL_CMD_H
#define SHELL_CMD_H

#include <kernel/shell/cli.h>

int cmd_pwd(cli_context_t *context, cli_cmd_t *cmds);
int cmd_cd(cli_context_t *context, cli_cmd_t *cmds);
int cmd_ls(cli_context_t *context, cli_cmd_t *cmds);
int cmd_clear(cli_context_t *context, cli_cmd_t *command);
int cmd_touch(cli_context_t *context, cli_cmd_t *command);
int cmd_mkdir(cli_context_t *context, cli_cmd_t *command);
int cmd_rm(cli_context_t *context, cli_cmd_t *command);
int cmd_rmdir(cli_context_t *context, cli_cmd_t *command);
int cmd_cat(cli_context_t *context, cli_cmd_t *command);
int cmd_write(cli_context_t *context, cli_cmd_t *command);
int cmd_history(cli_context_t *context, cli_cmd_t *command);
int cmd_help(cli_context_t *context, cli_cmd_t *command);
int cmd_edit(cli_context_t *context, cli_cmd_t *command);

#endif