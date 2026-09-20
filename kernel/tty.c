#include <asm.h>
#include <drivers/keyboard/keyboard.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

void tty_init(tty_t *tty, tty_output_t output) {
  tty->cursor = 0;
  tty->len = 0;
  tty->buffer[0] = '\0';
  tty->line_ready = false;
  tty->echo = true;
  tty->history = NULL;
  tty->history_count = 0;
  tty->history_index = 0;
  tty->output = output;
}

void tty_set_history(tty_t *tty, char **history, size_t history_count) {
  tty->history = history;
  tty->history_count = history_count;
  tty->history_index = history_count;
}

static void tty_replace_line(tty_t *tty, const char *line) {
  size_t length = strlen((char *)line);
  size_t old_length = tty->len;

  if (length >= TTY_BUFFER_SIZE)
    length = TTY_BUFFER_SIZE - 1;
  while (tty->cursor > 0) {
    tty->output.move_cursor(-1);
    tty->cursor--;
  }
  memcpy(tty->buffer, line, length);
  tty->buffer[length] = '\0';
  tty->len = (uint16_t)length;
  tty->cursor = (uint16_t)length;
  tty->output.write(tty->buffer);
  for (size_t i = length; i < old_length; i++)
    tty->output.putc(' ');
  for (size_t i = length; i < old_length; i++)
    tty->output.move_cursor(-1);
}

void tty_handle_event(tty_t *tty, const keyboard_event_t *event) {
  if (event->type != KEY_PRESS)
    return;

  char c = event->character;

  switch (event->key) {
  case UP_ARROW_KEY:
    if (tty->history != NULL && tty->history_index > 0) {
      tty->history_index--;
      tty_replace_line(tty, tty->history[tty->history_index]);
    }
    break;
  case DOWN_ARROW_KEY:
    if (tty->history != NULL && tty->history_index < tty->history_count) {
      tty->history_index++;
      if (tty->history_index == tty->history_count)
        tty_replace_line(tty, "");
      else
        tty_replace_line(tty, tty->history[tty->history_index]);
    }
    break;
  case ENTER_KEY:
    tty->line_ready = true;

    if (tty->echo)
      tty->output.putc('\n');

    break;

  case LEFT_ARROW_KEY:
    if (tty->cursor > 0) {
      tty->cursor--;

      if (tty->echo)
        tty->output.move_cursor(-1);
    }

    break;

  case RIGHT_ARROW_KEY:
    if (tty->cursor < tty->len) {
      tty->cursor++;

      if (tty->echo)
        tty->output.move_cursor(1);
    }

    break;

  case BACKSPACE_KEY:
    if (tty->cursor == 0)
      break;

    memmove(&tty->buffer[tty->cursor - 1], &tty->buffer[tty->cursor],
            tty->len - tty->cursor + 1);

    tty->cursor--;
    tty->len--;

    if (tty->echo) {
      tty->output.putc('\b');
      tty->output.write(&tty->buffer[tty->cursor]);
      tty->output.putc(' ');
      tty->output.move_cursor(-(tty->len - tty->cursor + 1));
    }

    break;

  default:
    if (!is_printable_key(event->key))
      break;

    if (tty->len >= TTY_BUFFER_SIZE - 1)
      break;

    memmove(&tty->buffer[tty->cursor + 1], &tty->buffer[tty->cursor],
            tty->len - tty->cursor + 1);

    tty->buffer[tty->cursor] = c;

    tty->cursor++;
    tty->len++;

    if (tty->echo) {
      tty->output.write(&tty->buffer[tty->cursor - 1]);
      tty->output.move_cursor(-(tty->len - tty->cursor));
    }

    break;
  }
}

void tty_write(tty_t *tty, const char *buf) { tty->output.write(buf); }

void tty_nwrite(tty_t *tty, const char *buf, size_t len) {
  tty->output.nwrite(buf, len);
}

void tty_putc(tty_t *tty, char c) { tty->output.putc(c); }

size_t tty_readline(tty_t *tty, char *buf, size_t buf_size) {
  while (!tty->line_ready) {
    keyboard_event_t *event = keyboard_pop_event();

    if (event != NULL) {
      tty_handle_event(tty, event);
    } else {
      hlt();
    }
  }

  size_t n = tty->len;

  if (n >= buf_size)
    n = buf_size - 1;

  memcpy(buf, tty->buffer, n);
  buf[n] = '\0';

  tty->len = 0;
  tty->cursor = 0;
  tty->buffer[0] = '\0';
  tty->line_ready = false;
  tty->history_index = tty->history_count;

  return n;
}

size_t tty_read(tty_t *tty, char *buf, size_t count) {
  size_t n = count < tty->len ? count : tty->len;

  memcpy(buf, tty->buffer, n);

  if (n < tty->len) {
    memmove(tty->buffer, &tty->buffer[n], tty->len - n + 1);
  }

  tty->len -= n;
  tty->cursor = tty->cursor > n ? tty->cursor - n : 0;

  return n;
}