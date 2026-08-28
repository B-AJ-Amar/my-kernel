#ifndef IO_H
#define IO_H

#include <interupts/idt.h>
#include <stdint.h>

// TODO: move the helpers into asm.c
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);

#endif