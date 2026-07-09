#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>
#include <stdint.h>

extern struct console_driver_t *active_console;

void console_set(struct console_driver_t *driver);
void console_putchar(char c);
void console_write(const char *str);
void console_clear(void);
void console_set_cursor(size_t row, size_t col);
void console_get_cursor(size_t *row, size_t *col);

typedef struct console_driver_t {
  void (*putchar)(char);
  void (*write)(const char *);
  void (*clear)(void);
  void (*set_cursor)(size_t, size_t);
  void (*get_cursor)(size_t *row, size_t *col);
  int (*getchar)(void);
} console_driver_t;

#endif