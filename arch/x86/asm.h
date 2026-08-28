#ifndef IO_H
#define IO_H

#include <interupts/idt.h>
#include <stdint.h>

#define KERNEL_CS 0x8
#define KERNEL_DS 0x10

void iasm();
inline void popad() { __asm__ volatile("popad"); }
inline void pushad() { __asm__ volatile("pushad"); }

void hlt();
void lidt(void *idt_ptr);
void enable_interrupts(void);
void disable_interrupts(void);
uintptr_t get_cr2(void);
uintptr_t get_cr3(void);

#endif