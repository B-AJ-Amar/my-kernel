
#include <isr.h>
#include <stdio.h>
#include <io.h>
// https://wiki.osdev.org/Exceptions
void isr_handler(struct registers *regs){
    printf("Interrupt %u:%u\n", regs->int_no, regs->err_code);
    hlt();
}