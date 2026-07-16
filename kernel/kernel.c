#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
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
  init_keyboard(KB_BACKEND_PS2, &layout_us);

  enable_interrupts();

  printf("\tHell  \bo %s the \nkernel\n", "from");

  // int x = 10 / 0;

  // __asm__ volatile("ud2");

  while (1) {
  }
}