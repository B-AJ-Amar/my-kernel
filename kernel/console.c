#include <kernel/console.h>

console_driver_t *active_console = NULL;

void console_set(console_driver_t *driver) {
  active_console = driver;
}


void console_putchar(char c) {
  if (active_console && active_console->putchar) {
    active_console->putchar(c);
  }
}

void console_write(const char *str) {
  if (active_console && active_console->write) {
    active_console->write(str);
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