

#include <idt.h>
#include <io.h>
#include <isr.h>
#include <stdint.h>
#include <string.h>

struct interrupt_descriptor idt[256];
struct idt_ptr idtr;

void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector,
                  uint8_t flags) {
  idt[num].offset_low = base & 0xFFFF;
  idt[num].selector = selector;
  idt[num].zero = 0;
  idt[num].flags = flags;
  idt[num].offset_high = (base >> 16) & 0xFFFF;
}

static const void *interupts[48] = {
    isr0,  isr1,  isr2,  isr3,  isr4,  isr5,  isr6,  isr7,  isr8,  isr9,  isr10,
    isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21,
    isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,

    irq0,  irq1,  irq2,  irq3,  irq4,  irq5,  irq6,  irq7,  irq8,  irq9,  irq10,
    irq11, irq12, irq13, irq14, irq15};

void init_idt(void) {
  memset(idt, 0, sizeof(idt));
  idtr.limit = sizeof(idt) - 1;
  idtr.base = (uint32_t)&idt;

  int flags = IDT_GATE_INTERRUPT;
  for (int i = 0; i < 48; i++) {
    if (i == 3 || i == 4)
      flags = IDT_GATE_TRAP;
    else
      flags = IDT_GATE_INTERRUPT;
    set_idt_gate(i, (uint32_t)interupts[i], KERNEL_CS, flags);
  }

  lidt(&idtr);
}