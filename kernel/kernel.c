#include <asm.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/ps2/controller.h>
#include <drivers/vga/vga.h>
#include <interupts/idt.h>
#include <interupts/pic.h>
#include <kernel/console.h>
#include <kernel/shed/shed.h>
#include <kernel/tty.h>
#include <mm/heap/heap.h>
#include <mm/mm.h>
#include <mm/pmm/boot_info.h>
#include <sleep.h>
#include <stdio.h>
#include <timer/pit.h>
// Extern declarations for kernel stack boundaries (defined in linker script)
extern char __stack_bottom;
extern char __stack_top;

void infinite_print(const char *str);
void shell(void);
void kernel(void) {

  disable_interrupts();
  idt_init();
  pic_init();

  pit_init(PIT_FREQUENCY);

  vga_init();
  console_set(&vga_console);

  ps2_init();
  keyboard_init(KB_BACKEND_PS2, &layout_us);

  tty_init(local_keyboard_input, local_console_output);

  
  boot_info_t *boot = (boot_info_t *)BOOT_INFO_ADDR;
  mm_init(boot,(uintptr_t)&__stack_top,(uintptr_t)&__stack_bottom);

  shed_init();
  
  enable_interrupts();

  task_create((void *)infinite_print, "Hello, World!\n");
  task_create((void *)infinite_print, "Bye, World!\n");
  // task_create((void *)shell, NULL);
  
  
  
  uint32_t *array = (uint32_t *)kmalloc(100);
  kfree(array);

  printf("kernel address: 0x%x\n", boot->kernel_addr);

printf("stack bottom: 0x%x\n", (uintptr_t)&__stack_bottom);
printf("stack top:    0x%x\n", (uintptr_t)&__stack_top);
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

void shell(void)
{
  keyboard_event_t *event;
  while (1) {

    // tty process
    while (tty_check_events()) {
      event = tty_read_event();
      tty_handle_event(event);
    }
  }
}

void infinite_print(const char *str) {
  uint8_t i = 0;
  while (i<10) {
    printf("[%u]%s", i++, str);
  }
  return;
}
