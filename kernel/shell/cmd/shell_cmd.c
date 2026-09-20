#include <asm.h>
#include <kernel/console.h>
#include <kernel/shell/cmd/cmd.h>
#include <kernel/shell/shell.h>
#include <mm/pmm/pmm.h>
#include <stdio.h>
#include <string.h>
#include <timer/pit.h>

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
      (command->arg_count == 1 && !parse_offset(command->args[0], &offset)))
    return CLI_ERROR;

  cli_print_help(offset, shell_highlight_color);
  return CLI_SUCCESS;
}

int cmd_print(cli_context_t *context, cli_cmd_t *command) {
  (void)context;

  for (size_t i = 0; i < command->arg_count; i++) {
    if (i != 0)
      putchar(' ');
    printf("%s", command->args[i]);
  }
  putchar('\n');
  return CLI_SUCCESS;
}

int cmd_info(cli_context_t *context, cli_cmd_t *command) {
  uint32_t max_leaf;
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t edx;
  char vendor[13];

  (void)context;
  (void)command;
  printf("%sKernel\033[15,0]      my-kernel 1.0.0\n", shell_highlight_color);
  printf("%sArch\033[15,0]        x86 32-bit\n", shell_highlight_color);
  printf("%sMemory\033[15,0]      %llu MB\n", shell_highlight_color,
         pmm_get_free_memory() / (1024ULL * 1024ULL));
  cpuid(0, &max_leaf, &ebx, &ecx, &edx);
  memcpy(&vendor[0], &ebx, sizeof(ebx));
  memcpy(&vendor[4], &edx, sizeof(edx));
  memcpy(&vendor[8], &ecx, sizeof(ecx));
  vendor[12] = '\0';
  printf("%sCPU vendor\033[15,0]  %s\n", shell_highlight_color, vendor);
  if (max_leaf >= 1) {
    cpuid(1, &eax, &ebx, &ecx, &edx);
    printf("%sSignature\033[15,0]   0x%x\n", shell_highlight_color, eax);
    printf("%sFeatures\033[15,0]    0x%x\n", shell_highlight_color, edx);
  } else {
    printf("%sFeatures\033[15,0]     unavailable\n", shell_highlight_color);
  }
  return CLI_SUCCESS;
}

int cmd_uptime(cli_context_t *context, cli_cmd_t *command) {
  uint64_t ticks;

  (void)context;
  (void)command;
  ticks = pit_get_ticks();
  printf("Uptime: %u seconds\n", (unsigned int)(ticks / PIT_FREQUENCY));
  return CLI_SUCCESS;
}
