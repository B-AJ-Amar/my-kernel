#ifndef TTY_H
#define TTY_H
#define TTY_BUFFER_SIZE 256
#include <drivers/keyboard/keyboard.h>
#include <stddef.h>
#include <stdint.h>

// it can be local (ps2/usb .. device) or ssh for example
typedef struct tty_input {
  bool (*read_event)(keyboard_event_t *event);
} tty_input;

typedef struct {
  void (*putc)(char c);
  void (*write)(const char *buf, size_t len);
} tty_output_t;

typedef struct {
  char buffer[TTY_BUFFER_SIZE];
  uint16_t cursor;
  uint16_t len;
  bool echo; // if true: echo input to output
  tty_input input;
  tty_output_t output;

  char *(*read_line)(char *buf, size_t buf_size);

} tty_t;

void tty_init(tty_t *tty);
size_t tty_read(tty_t *tty, char *buf, size_t count);
size_t tty_readline(tty_t *tty, char *buf, size_t buf_size);
void tty_write(tty_t *tty, const char *buf, size_t len);
void tty_handle_event(tty_t *tty, const keyboard_event_t *event);

#endif