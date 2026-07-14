#ifndef IRQ_H
#define IRQ_H

#include "isr.h"
#include <interupt.h>

void irq_handler(interupt_registers_t *regs);

#endif