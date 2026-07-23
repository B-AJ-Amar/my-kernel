#include <kernel/console.h>
#include <kernel/tty.h>

console_driver_t *active_console = NULL;

void console_set(console_driver_t *driver) { active_console = driver; }

void console_putchar(char c) {
  if (active_console && active_console->putchar) {
    active_console->putchar(c);
  }
}

void console_clear(void) {
  if (active_console && active_console->clear) {
    active_console->clear();
  }
}

void console_set_cursor(size_t row, size_t col) {
  if (active_console && active_console->set_cursor) {
    active_console->set_cursor(row, col);
  }
}

void console_get_cursor(size_t *row, size_t *col) {
  if (active_console && active_console->get_cursor) {
    active_console->get_cursor(row, col);
  }
}

void console_set_color(int fg, int bg) {
  if (active_console && active_console->set_color) {
    active_console->set_color(fg, bg);
  }
}

/* parses "\033[<fg>,<bg>]", returns pointer past the ']' on success,
 * or NULL if str doesn't hold a valid sequence */
static const char *console_parse_color(const char *str) {
  const char *p = str + 2;
  int fg = 0, bg = 0;
  int had_digit = 0;

  while (*p >= '0' && *p <= '9') {
    fg = fg * 10 + (*p - '0');
    had_digit = 1;
    p++;
  }
  if (!had_digit || *p != ',') {
    return NULL;
  }
  p++;

  had_digit = 0;
  while (*p >= '0' && *p <= '9') {
    bg = bg * 10 + (*p - '0');
    had_digit = 1;
    p++;
  }
  if (!had_digit || *p != ']') {
    return NULL;
  }
  p++;

  console_set_color(fg, bg);
  return p;
}

void console_write(const char *str) {
  if (!active_console) {
    return;
  }

  while (*str) {
    if (str[0] == '\033' && str[1] == '[') {
      const char *next = console_parse_color(str);

      if (next) {
        str = next;
        continue;
      }
    }

    console_putchar(*str);
    str++;
  }
}

void console_nwrite(const char *str, size_t len) {
  if (!active_console) {
    return;
  }

  for (size_t i = 0; i < len; i++) {
    console_putchar(str[i]);
  }
}

void console_move_cursor(int offset) {
  if (!active_console) {
    return;
  }

  active_console->move_cursor(offset);
}