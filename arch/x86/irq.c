#include <irq.h>
#include <pic.h>
#include <stdio.h>
#include <io.h>
#include <drivers/keyboard/keyboard.h>
// TODO: keyboard+ time drivers
static void __irq_keyboard_handler__();


void irq_handler(interupt_registers_t *regs) {
  switch (regs->int_no) {
  case 33:
    __irq_keyboard_handler__();
    break;

  case 32:
    // timer_handler();
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
