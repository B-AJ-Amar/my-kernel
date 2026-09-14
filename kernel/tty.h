#ifndef TTY_H
#define TTY_H

#include <drivers/keyboard/keyboard.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TTY_BUFFER_SIZE 512

typedef struct {
  void (*putc)(char c);
  void (*write)(const char *buf);
  void (*nwrite)(const char *buf, size_t len);
  void (*move_cursor)(int offset);
} tty_output_t;

typedef struct {
  char buffer[TTY_BUFFER_SIZE];
  uint16_t cursor;
  uint16_t len;
  bool echo;
  bool line_ready;
  tty_output_t output;
} tty_t;

void tty_init(tty_t *tty, tty_output_t output);

void tty_handle_event(tty_t *tty, const keyboard_event_t *event);

size_t tty_read(tty_t *tty, char *buf, size_t count);
size_t tty_readline(tty_t *tty, char *buf, size_t buf_size);

void tty_putc(tty_t *tty, char c);
void tty_write(tty_t *tty, const char *buf);
void tty_nwrite(tty_t *tty, const char *buf, size_t len);

#endif