#ifndef IRQ_H
#define IRQ_H

#include <interupt.h>
#include <isr.h>

void irq_handler(interupt_registers_t *regs);

#endif