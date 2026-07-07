#include <stdio.h>
#include <drivers/vga.h>
#include <idt.h>
void kernel(void) {
  init_idt();
  vga_init();
  console_set(&vga_console);
  printf("\tHell \bo %s the \nkernel", "from");
  
  // int x = 10 / 0;

  // __asm__ volatile ("ud2");
  

  while (1) {
  }
}