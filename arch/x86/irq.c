#include <drivers/keyboard/keyboard.h>
#include <io.h>
#include <irq.h>
#include <pic.h>
#include <pit.h>
#include <stdio.h>
// TODO: keyboard+ time drivers
static void __irq_keyboard_handler__();
static void __irq_timer_handler__();

void irq_handler(interupt_registers_t *regs) {
  switch (regs->int_no) {
  case 33:
    __irq_keyboard_handler__();
    break;

  case 32:
    __irq_timer_handler__();
    break;
  }

  pic_send_eoi(regs->int_no - PIC1_OFFSET); // end of interupt
}

static void __irq_keyboard_handler__() {
  keyboard_driver_t *driver = keyboard_get_driver();
  if (driver && driver->interupt_handler) {
    driver->interupt_handler();
  }
}

static void __irq_timer_handler__() { pit_interrupt_handler(); }
