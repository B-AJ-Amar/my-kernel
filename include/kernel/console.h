#ifndef CONSOLE_H
#define CONSOLE_H
#include <kernel/tty.h>
#include <stddef.h>
#include <stdint.h>

extern struct console_driver_t *active_console;

void console_set(struct console_driver_t *driver);
void console_putchar(char c);
void console_write(const char *str);
void console_nwrite(const char *str, size_t len);
void console_move_cursor(int offset);
void console_clear(void);
void console_set_cursor(size_t row, size_t col);
void console_get_cursor(size_t *row, size_t *col);
void console_set_color(int fg, int bg);

typedef struct console_driver_t {
  void (*putchar)(char);
  void (*write)(const char *);
  void (*nwrite)(const char *, size_t);
  void (*clear)(void);
  void (*set_cursor)(size_t, size_t);
  void (*get_cursor)(size_t *row, size_t *col);
  void (*set_color)(int fg, int bg);
  int (*getchar)(void);
  void (*move_cursor)(int offset); // e.g -1 for left, +1 for right
} console_driver_t;

#endif