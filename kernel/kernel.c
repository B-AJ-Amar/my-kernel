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
  keyboard_event_t *event;
  while (1) {
    printf("\033[15,0]");
    while ((event = keyboard_peek_event()) != NULL) {
      event = keyboard_pop_event();
      tty_handle_event(&tty0, event);
    }
  }
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
  fs_init();
  // fs_touch("/", "test_file.txt");
  // fs_mkdir("/", "amar_test");
  // fs_base_node_t *list = fs_ls("/");
  // for (size_t i = 0; list[i].name[0] != '\0'; i++) {
  //   char *color = (list[i].type == FS_DIR) ? "\033[3,0]m" : "\033[15,0]";
  //   printf("fs_ls: %s%s\n", color, list[i].name );
  // }
  shed_init();
  enable_interrupts();

  task_create(task_a, NULL);
  task_create(shell, NULL);

  uintptr_t sp;
  __asm__ volatile("mov %%esp, %0" : "=r"(sp));
  printf("Current stack pointer: 0x%lx\n", sp);

  uint32_t *array = (uint32_t *)kmalloc(100);
  kfree(array);

  printf("kernel address: 0x%x\n", boot->kernel_addr);

  printf("stack bottom: 0x%x\n", (uintptr_t)&__stack_bottom);
  printf("stack top:    0x%x\n", (uintptr_t)&__stack_top);
  printf("\033[1,4] Hello from the kernel\n");
  printf("\033[2,0] Hello from the kernel\n");
  printf("\033[3,0] Hello from the kernel\n");
  while (1)
    ;
}
