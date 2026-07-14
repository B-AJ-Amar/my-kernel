#ifndef INTERUPT_H
#define INTERUPT_H

#include <stdint.h>

typedef struct {
  // ? pusha
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  // ? pushed by me (in irs.asm)
  uint32_t int_no;
  uint32_t err_code;

  // ? pushed by the cpu automatically
  uint32_t eip;
  uint32_t cs;
  uint32_t eflags;
} interupt_registers_t;

extern void interrupt_handler(interupt_registers_t *regs);

#endif