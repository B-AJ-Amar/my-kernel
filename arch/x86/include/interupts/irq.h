#ifndef IRQ_H
#define IRQ_H

#include <interupts/interupt.h>
#include <interupts/isr.h>

void irq_handler(interupt_registers_t *regs);

#endif