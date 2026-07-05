#include <stdio.h>
#include <drivers/vga.h>

void kernel(void) {
  vga_init();
  console_set(&vga_console);
  printf("\tHell \bo %s the \nkernel", "from");

  while (1) {
  }
}