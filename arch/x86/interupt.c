
#include <interupt.h>
#include <io.h>
#include <irq.h>
#include <isr.h>
#include <stdio.h>

// https://wiki.osdev.org/Exceptions

void interupt_handler(interupt_registers_t *regs) {
  if (regs->int_no < 32)
    isr_handler(regs);
  else if (regs->int_no >= 32 && regs->int_no < 48)
    irq_handler(regs);
  else
    panic("Unknown interupt: %u\n", regs->int_no);
}