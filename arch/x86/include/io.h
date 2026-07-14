#ifndef IO_H
#define IO_H

#include <idt.h>
#include <stdint.h>

#define KERNEL_CS 0x8
#define KERNEL_DS 0x10

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

void hlt();
void lidt(void *idt_ptr);
void enable_interrupts(void);
void disable_interrupts(void);

#endif