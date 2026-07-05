#include <drivers/vga.h>

void kernel(void) {
  vga_init();
  console_set(&vga_console);
  console_write("\tHell \bo from the \nkernel");

  while (1) {
  }
}