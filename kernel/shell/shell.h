#ifndef SHELL_H
#define SHELL_H

#include <kernel/tty.h>

extern const char *shell_highlight_color;

void shell_init(void);
void shell_run(tty_t *tty);

#endif