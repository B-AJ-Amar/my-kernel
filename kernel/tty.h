#ifndef TTY_H
#define TTY_H
#define TTY_BUFFER_SIZE 512
#include <drivers/keyboard/keyboard.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
// it can be local (ps2/usb .. device) or ssh for example

typedef struct tty_input_t {
  keyboard_event_t *(*read_event)(void);
  keyboard_event_t *(*check_events)(void);
} tty_input_t;

typedef struct {
  void (*putc)(char c);
  void (*write)(const char *buf);
  void (*nwrite)(const char *buf, size_t len);
  void (*move_cursor)(int offset); // e.g -1 for left, +1 for right
} tty_output_t;

typedef struct {
  char buffer[TTY_BUFFER_SIZE];
  uint16_t cursor;
  uint16_t len;
  bool echo; // if true: echo input to output
  bool line_ready;
  tty_input_t input;
  tty_output_t output;

} tty_t;

extern tty_input_t local_keyboard_input;
extern tty_output_t local_console_output;

void tty_init(tty_input_t input, tty_output_t output);
size_t tty_read(char *buf, size_t count);
size_t tty_readline(char *buf, size_t buf_size);
void tty_putc(char c);
void tty_write(const char *buf);
void tty_nwrite(const char *buf, size_t len);
void tty_handle_event(const keyboard_event_t *event);
keyboard_event_t *tty_read_event(void);
keyboard_event_t *tty_check_events(void);

#endif