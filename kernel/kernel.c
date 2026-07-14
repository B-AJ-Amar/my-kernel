#include <drivers/ps2/controller.h>
#include <drivers/vga/vga.h>
#include <idt.h>
#include <io.h>
#include <pic.h>
#include <stdio.h>
__attribute__((section(".start"))) void kernel(void) {

  disable_interrupts();
  init_idt();
  pic_init();
  
  vga_init();
  console_set(&vga_console);
  ps2_init();
  printf("\tHell  \bo %s the \nkernel\n", "from");
  enable_interrupts();

  
  // outb(0x43, 0x36);

  // uint16_t divisor = 1193182 / 100;


  // outb(0x40, divisor & 0xFF);
  // outb(0x40, divisor >> 8);

  // int x = 10 / 0;

  // __asm__ volatile("ud2");

  while (1) {
  }
}