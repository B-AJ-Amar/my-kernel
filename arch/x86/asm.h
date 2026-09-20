#ifndef ASM_H
#define ASM_H

#include <interupts/idt.h>
#include <stdint.h>

#define KERNEL_CS 0x8
#define KERNEL_DS 0x10

#define CODE_SEG KERNEL_CS
#define DATA_SEG KERNEL_DS
#define DEFAULT_EFLAGS 0x202 // IF=1, bit 1 is always 1

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

static inline void cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx,
                         uint32_t *ecx, uint32_t *edx) {
  __asm__ volatile("cpuid"
                   : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                   : "a"(leaf));
}

#endif