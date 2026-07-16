#include <io.h>

void outb(uint16_t port, uint8_t value) {
  __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

uint8_t inb(uint16_t port) {
  uint8_t value;

  __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port));

  return value;
}

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