#include <interupts/interupt.h>
#include <interupts/isr.h>
#include <stdio.h>
#include <io.h>

const char *exception_names[32] = {"Divide Error",
                                   "Debug",
                                   "Non-Maskable Interrupt",
                                   "Breakpoint",
                                   "Overflow",
                                   "BOUND Range Exceeded",
                                   "Invalid Opcode",
                                   "Device Not Available",
                                   "Double Fault",
                                   "Coprocessor Segment Overrun",
                                   "Invalid TSS",
                                   "Segment Not Present",
                                   "Stack Segment Fault",
                                   "General Protection Fault",
                                   "Page Fault",
                                   "Reserved",
                                   "x87 Floating-Point Exception",
                                   "Alignment Check",
                                   "Machine Check",
                                   "SIMD Floating-Point Exception",
                                   "Virtualization Exception",
                                   "Control Protection Exception",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Hypervisor Injection Exception",
                                   "VMM Communication Exception",
                                   "Security Exception",
                                   "Reserved"};

void isr_handler(interupt_registers_t *regs) {
  // todo: add a better handler for page fault
  if (regs->int_no == 14) {
    uint32_t faulting_address = get_cr2();
    panic("Exception %u:%u : Page Fault at address: 0x%x\n", regs->int_no, regs->err_code, faulting_address);
  }
  panic("Exception %u:%u : %s\n", regs->int_no, regs->err_code,
        exception_names[regs->int_no]);
}