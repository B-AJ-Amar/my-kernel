#ifndef IO_H
#define IO_H

#include <interupts/idt.h>
#include <stdint.h>

void iasm();

void hlt() { __asm__ volatile("hlt"); }

void lidt(void *idt_ptr) {
  __asm__ volatile(".intel_syntax noprefix\n\t"
                   "lidt [%0]\n\t"
                   ".att_syntax prefix"
                   :
                   : "r"(idt_ptr));
}

void enable_interrupts(void) { __asm__ volatile("sti"); }
void disable_interrupts(void) { __asm__ volatile("cli"); }

uintptr_t get_cr2(void) {
  uintptr_t cr2;
  __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
  return cr2;
}
uintptr_t get_cr3(void) {
  uintptr_t cr3;
  __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
  return cr3;
}

#endif