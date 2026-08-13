#ifndef VGA_H
#define VGA_H

#include <kernel/console.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern console_driver_t vga_console;
enum vga_color;
void vga_init(void);
void vga_clear(void);
void vga_reset_color(void);
void vga_putchar(char c);
void vga_write(const char *str);
void vga_set_color(int fg, int bg);
void vga_console_set_color(int fg, int bg);
void vga_set_cursor(size_t row, size_t col);
void vga_get_cursor(size_t *row, size_t *col);
void vga_move_cursor(int offset);

#endif