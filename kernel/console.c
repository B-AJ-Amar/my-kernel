#include <console.h>


void console_set(console_driver_t *driver) {
  console = driver;
}


void console_putchar(char c) {
  if (console && console->putchar) {
    console->putchar(c);
  }
}

void console_write(const char *str) {
  if (console && console->write) {
    console->write(str);
  }
}

void console_clear(void) {
  if (console && console->clear) {
    console->clear();
  }
}

void console_set_cursor(size_t row, size_t col) {
  if (console && console->set_cursor) {
    console->set_cursor(row, col);
  }
}

void console_get_cursor(size_t *row, size_t *col) {
  if (console && console->get_cursor) {
    console->get_cursor(row, col);
  }
}