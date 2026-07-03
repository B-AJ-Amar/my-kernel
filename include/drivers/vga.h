#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stdbool.h>

void vga_clear(void);
void vga_reset_color(void);
void vga_putchar(char c);
void vga_write(const char* str);
void vga_set_color(uint8_t fg, uint8_t bg);

#endif