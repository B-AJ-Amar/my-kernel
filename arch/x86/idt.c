

#include <idt.h>
#include <io.h>
#include <stdint.h>
#include <string.h>
#include <isr.h>

struct interrupt_descriptor idt[256];
struct idt_ptr idtr;



void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].offset_low = base & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void init_idt(void){
    memset(idt, 0, sizeof(idt));
    idtr.limit = sizeof(idt)-1;
    idtr.base = (uint32_t)&idt;

    set_idt_gate(0,(uint32_t)isr0,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(1,(uint32_t)isr1,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(2,(uint32_t)isr2,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(3,(uint32_t)isr3,KERNEL_CS,IDT_GATE_TRAP);
    set_idt_gate(4,(uint32_t)isr4,KERNEL_CS,IDT_GATE_TRAP);
    set_idt_gate(5,(uint32_t)isr5,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(6,(uint32_t)isr6,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(7,(uint32_t)isr7,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(8,(uint32_t)isr8,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(9,(uint32_t)isr9,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(10,(uint32_t)isr10,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(11,(uint32_t)isr11,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(12,(uint32_t)isr12,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(13,(uint32_t)isr13,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(14,(uint32_t)isr14,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(15,(uint32_t)isr15,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(16,(uint32_t)isr16,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(17,(uint32_t)isr17,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(18,(uint32_t)isr18,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(19,(uint32_t)isr19,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(20,(uint32_t)isr20,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(21,(uint32_t)isr21,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(22,(uint32_t)isr22,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(23,(uint32_t)isr23,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(24,(uint32_t)isr24,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(25,(uint32_t)isr25,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(26,(uint32_t)isr26,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(27,(uint32_t)isr27,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(28,(uint32_t)isr28,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(29,(uint32_t)isr29,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(30,(uint32_t)isr30,KERNEL_CS,IDT_GATE_INTERRUPT);
    set_idt_gate(31,(uint32_t)isr31,KERNEL_CS,IDT_GATE_INTERRUPT);

    lidt(&idtr);


}