#ifndef VGA_H
#define VGA_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <console.h>    

extern console_driver_t vga_console;

void vga_init(void);
void vga_clear(void);
void vga_reset_color(void);
void vga_putchar(char c);
void vga_write(const char *str);
void vga_set_color(uint8_t fg, uint8_t bg);
void vga_set_cursor(size_t row, size_t col);
void vga_get_cursor(size_t *row, size_t *col);


#endif