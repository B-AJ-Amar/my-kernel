#include <asm.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
// TODO: add readline queue to store multiple lines and read them one by one
static tty_t local_tty;

tty_input_t local_keyboard_input = {
    .read_event = keyboard_pop_event,
    .check_events = keyboard_peek_event,
};

tty_output_t local_console_output = {
    .putc = console_putchar,
    .write = console_write,
    .nwrite = console_nwrite,
    .move_cursor = console_move_cursor,
};

void tty_init(tty_input_t input, tty_output_t output) {
  local_tty.cursor = 0;
  local_tty.len = 0;
  local_tty.buffer[0] = '\0';
  local_tty.line_ready = false;
  local_tty.echo = true;
  local_tty.input = input;
  local_tty.output = output;
}

void tty_handle_event(const keyboard_event_t *event) {
  if (event->type != KEY_PRESS)
    return;

  char c = event->character;

  switch (event->key) {

  case ENTER_KEY:
    // printf("buffer: `%s`\n", local_tty.buffer);
    local_tty.line_ready = true;
    if (local_tty.echo)
      local_tty.output.putc('\n');
    break;

  case LEFT_ARROW_KEY:
    if (local_tty.cursor > 0) {
      local_tty.cursor--;
      if (local_tty.echo)
        local_tty.output.move_cursor(-1);
    }
    break;

  case RIGHT_ARROW_KEY:
    if (local_tty.cursor < local_tty.len) {
      if (local_tty.echo)
        local_tty.output.move_cursor(1);

      local_tty.cursor++;
    }
    break;

  case BACKSPACE_KEY:
    if (local_tty.cursor == 0)
      break;

    /* Shift everything left. */
    memmove(&local_tty.buffer[local_tty.cursor - 1],
            &local_tty.buffer[local_tty.cursor],
            local_tty.len - local_tty.cursor + 1);

    local_tty.cursor--;
    local_tty.len--;

    if (local_tty.echo) {
      local_tty.output.putc('\b');

      local_tty.output.write(&local_tty.buffer[local_tty.cursor]);
      local_tty.output.putc(' ');

      // move cursor back
      local_tty.output.move_cursor(-(local_tty.len - local_tty.cursor + 1));
    }
    break;

  default:
    if (!is_printable_key(event->key))
      break;

    if (local_tty.len >= TTY_BUFFER_SIZE - 1)
      break;

    // insert char
    memmove(&local_tty.buffer[local_tty.cursor + 1],
            &local_tty.buffer[local_tty.cursor],
            local_tty.len - local_tty.cursor + 1);

    local_tty.buffer[local_tty.cursor] = c;

    local_tty.cursor++;
    local_tty.len++;

    if (local_tty.echo) {
      // print the characters from the cursor to the end
      local_tty.output.write(&local_tty.buffer[local_tty.cursor - 1]);

      // restore the cursor position
      local_tty.output.move_cursor(-(local_tty.len - local_tty.cursor));
    }

    break;
  }
}

void tty_write(const char *buf) { local_tty.output.write(buf); }

void tty_nwrite(const char *buf, size_t len) {
  local_tty.output.nwrite(buf, len);
}

void tty_putc(char c) { local_tty.output.putc(c); }

size_t tty_readline(char *buf, size_t buf_size) {
  while (!local_tty.line_ready)
    hlt(); // sleep untill the next interupt

  size_t n = local_tty.len;

  if (n >= buf_size)
    n = buf_size - 1;

  memcpy(buf, local_tty.buffer, n);
  buf[n] = '\0';

  local_tty.len = 0;
  local_tty.cursor = 0;
  local_tty.buffer[0] = '\0';
  local_tty.line_ready = false;

  return n;
}

size_t tty_read(char *buf, size_t count) {
  size_t n = (count < local_tty.len) ? count : local_tty.len;
  memcpy(buf, local_tty.buffer, n);
  return n;
}

keyboard_event_t *tty_read_event(void) { return local_tty.input.read_event(); }

keyboard_event_t *tty_check_events(void) {
  return local_tty.input.check_events();
}