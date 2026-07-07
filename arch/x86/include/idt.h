#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_GATE_INTERRUPT 0x8E
#define IDT_GATE_TRAP      0x8F

struct interrupt_descriptor {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern struct interrupt_descriptor idt[256];
extern struct idt_ptr idtr;

void load_idt(struct idt_ptr *idtr);
void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void init_idt();
#endif