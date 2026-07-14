#include "irq.h"
#include "pic.h"

// TODO: keyboard+ time drivers
void irq_handler(interupt_registers_t *regs) {
  switch (regs->int_no) {
  case 33:
    // keyboard_handler();
    break;

  case 32:
    // timer_handler();
    break;
  }

  pic_send_eoi(regs->int_no - PIC1_OFFSET); // end of interupt
}

void keyboard_handler() {}
