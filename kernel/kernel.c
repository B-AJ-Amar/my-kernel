#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/ps2/controller.h>
#include <drivers/vga/vga.h>
#include <idt.h>
#include <io.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <pic.h>
#include <pit.h>
#include <sleep.h>
#include <stdio.h>
__attribute__((section(".start"))) void kernel(void) {

  disable_interrupts();
  init_idt();
  pic_init();

  pit_init(PIT_FREQUENCY);

  vga_init();
  console_set(&vga_console);

  ps2_init();
  init_keyboard(KB_BACKEND_PS2, &layout_us);

  tty_init(local_keyboard_input, local_console_output);

  enable_interrupts();

  printf("\033[1,4] Hello from the kernel\n");
  printf("\033[2,0] Hello from the kernel\n");
  printf("\033[3,0] Hello from the kernel\n");
  sleep(3000);
  printf("\033[4,0] Hello from the kernel\n");

  keyboard_event_t *event;

  // todo: scredular
  while (1) {

    // tty process
    while (tty_check_events()) {
      event = tty_read_event();
      tty_handle_event(event);
    }
  }
}
