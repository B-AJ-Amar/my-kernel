#include <drivers/vga.h>

void kernel(void) {
  vga_clear();
  vga_write("\tHell \bo from the \nkernel");

  while (1) {
  }
}