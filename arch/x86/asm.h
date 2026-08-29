#ifndef ASM_H
#define ASM_H

#include <interupts/idt.h>
#include <stdint.h>

#define KERNEL_CS 0x8
#define KERNEL_DS 0x10

void iasm();
static inline void popad() {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "popad\n\t"
                   ".att_syntax prefix");
}
static inline void pushad() {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "pushad\n\t"
                   ".att_syntax prefix");
}

static inline uintptr_t get_sp(void) {
  uintptr_t sp;
  asm volatile("mov %%esp, %0" : "=r"(sp));
  return sp;
}

static inline void set_sp(uintptr_t sp) {
  asm volatile("mov %0, %%esp" : : "r"(sp) : "memory");
}

void hlt();
void lidt(void *idt_ptr);
void enable_interrupts(void);
void disable_interrupts(void);
uintptr_t get_cr2(void);
uintptr_t get_cr3(void);

#endif