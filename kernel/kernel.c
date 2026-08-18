#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/ps2/controller.h>
#include <drivers/vga/vga.h>
#include <interupts/idt.h>
#include <interupts/pic.h>
#include <io.h>
#include <kernel/console.h>
#include <kernel/tty.h>
#include <pmm/boot_info.h>
#include <pmm/pmm.h>
#include <sleep.h>
#include <stdio.h>
#include <timer/pit.h>
#include <vmm/vmm.h>

__attribute__((section(".start"))) void kernel(void) {

  boot_info_t *boot = (boot_info_t *)BOOT_INFO_ADDR;
  pmm_init(boot->kernel_addr, boot->kernel_size, boot->e820_entries_count,
           boot->e820_entries_addr);

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
  vmm_init();

  printf("kernel address: 0x%x\n", boot->kernel_addr);
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
