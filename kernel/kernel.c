#include <drivers/vga.h>

void kernel(void) {
  vga_clear();
  vga_write("Hello from the kernel");

  while (1) {
  }
}