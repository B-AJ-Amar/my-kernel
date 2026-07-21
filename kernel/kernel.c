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

  vga_set_color(2, 0);

  printf("\033[1,4] Hello from the kernel\n");
  printf("\033[2,0] Hello from the kernel\n");
  printf("\033[3,0] Hello from the kernel\n");
  printf("\033[4,0] Hello from the kernel\n");


  keyboard_event_t *event;

  // todo: scredular
  while (1) {
    while (keyboard_peek_event()) {
      event = keyboard_pop_event();
      if (event->type == KEY_PRESS) {
        printf("%c", event->character);
      }
    }
  }
}