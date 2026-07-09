#include <drivers/vga.h>
#include <idt.h>
#include <pic.h>
#include <stdio.h>
void kernel(void) {

  init_idt();
  pic_init();
  vga_init();
  console_set(&vga_console);

  printf("\tHell \bo %s the \nkernel\n", "from");

  // int x = 10 / 0;

  __asm__ volatile("ud2");

  while (1) {
  }
}