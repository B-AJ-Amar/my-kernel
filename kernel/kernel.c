#include <asm.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/ps2/controller.h>
#include <drivers/vga/vga.h>
#include <fs/fs.h>
#include <interupts/idt.h>
#include <interupts/pic.h>
#include <kernel/console.h>
#include <kernel/shed/shed.h>
#include <kernel/shell/shell.h>
#include <kernel/tty.h>
#include <mm/heap/heap.h>
#include <mm/mm.h>
#include <mm/pmm/boot_info.h>
#include <sleep.h>
#include <stdio.h>
#include <timer/pit.h>

extern char __stack_bottom;
extern char __stack_top;

static tty_t tty0;

void task_a(void *p) {
  printf("\033[2,0] Task A started\n");
  while (1) {
    printf("\033[2,0]A");
    for (volatile int i = 0; i < 1000000; i++)
      ;
  }
}

void shell(void *p) {
  shell_init();
  shell_run(&tty0);
}
void kernel(void) {

  disable_interrupts();
  idt_init();
  pic_init();

  pit_init(PIT_FREQUENCY);

  vga_init();
  console_set(&vga_console);

  ps2_init();
  keyboard_init(KB_BACKEND_PS2, &layout_us);
  tty_output_t local_console_output = {
      .putc = console_putchar,
      .write = console_write,
      .nwrite = console_nwrite,
      .move_cursor = console_move_cursor,
  };
  tty_init(&tty0, local_console_output);

  boot_info_t *boot = (boot_info_t *)BOOT_INFO_ADDR;
  mm_init(boot, (uintptr_t)&__stack_top, (uintptr_t)&__stack_bottom);
  printf("[\033[2,0]x\033[15,0]] Memory management initialized\n");
  fs_init();
  printf("[\033[2,0]x\033[15,0]] File system initialized\n");

  shed_init();
  printf("[\033[2,0]x\033[15,0]] Scheduler initialized\n");
  enable_interrupts();

  printf("[\033[2,0]x\033[15,0]]kernel address: 0x%x\n", boot->kernel_addr);

  console_clear();
  task_create(shell, NULL);
  // task_create(task_a, NULL);

  __block_thread__();
}
