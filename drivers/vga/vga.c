#include <drivers/vga/vga.h>
#include <io.h>
#include <kernel/console.h>
#include <string.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
#define TAB_SIZE 4
// TODO:add specal chars support (e.g \n \t ...)
// TODO: add move cursor and insert in the cusor
enum vga_color {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE,
  VGA_COLOR_GREEN,
  VGA_COLOR_CYAN,
  VGA_COLOR_RED,
  VGA_COLOR_MAGENTA,
  VGA_COLOR_BROWN,
  VGA_COLOR_LIGHT_GREY,
  VGA_COLOR_DARK_GREY,
  VGA_COLOR_LIGHT_BLUE,
  VGA_COLOR_LIGHT_GREEN,
  VGA_COLOR_LIGHT_CYAN,
  VGA_COLOR_LIGHT_RED,
  VGA_COLOR_LIGHT_MAGENTA,
  VGA_COLOR_LIGHT_BROWN,
  VGA_COLOR_WHITE
};
static uint16_t *const vga_buffer = (uint16_t *)VGA_MEMORY;
static uint8_t color = VGA_COLOR_BLACK << 4 | VGA_COLOR_WHITE;
static uint8_t row = 0;
static uint8_t col = 0;

static void scroll(void);
static void update_cursor(void);
static void vga_update_cursor(uint16_t pos);
static void reset_cursor(void);
static inline uint16_t get_cursor(void);
static inline uint16_t vga_char(char character, uint8_t color);
static void disable_cursor();
static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);

console_driver_t vga_console = {.putchar = vga_putchar,
                                .write = vga_write,
                                .clear = vga_clear,
                                .set_cursor = vga_set_cursor,
                                .get_cursor = vga_get_cursor,
                                .getchar = NULL};

void vga_init(void) {
  row = 0;
  col = 0;
  enable_cursor(0, 15);
  vga_reset_color();
  vga_clear();
}

void vga_set_color(uint8_t fg, uint8_t bg) { color = bg << 4 | fg; }

void vga_reset_color(void) { color = VGA_COLOR_BLACK << 4 | VGA_COLOR_WHITE; }

void vga_clear(void) {
  vga_reset_color();
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_putchar(' ');
  }
  reset_cursor();
  vga_update_cursor(0);
}

uint16_t vga_char(char character, uint8_t color) {
  return ((uint16_t)color << 8) | (uint8_t)character;
}

static void update_cursor(void) {
  if (col < VGA_WIDTH - 1)
    col++;
  else {
    col = 0;
    if (row < VGA_HEIGHT - 1)
      row++;
    else
      scroll();
  }
  vga_update_cursor(get_cursor());
}

/*
0x3d4 : selector
0x3D5 : write/read
0x0f  : low cursor
0x0e  : high cursor

*/

void vga_update_cursor(uint16_t pos) {
  outb(0x3D4, 0x0F);
  outb(0x3D5, pos & 0xFF);

  outb(0x3D4, 0x0E);
  outb(0x3D5, pos >> 8);
}

static uint16_t get_cursor(void) { return row * VGA_WIDTH + col; }

void reset_cursor(void) {
  col = 0;
  row = 0;
}

void vga_write(const char *str) {
  while (*str)
    vga_putchar(*str++);
}

void disable_cursor() {
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void scroll(void) {
  memmove(vga_buffer, vga_buffer + VGA_WIDTH,
          (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16_t));
  for (int i = 0; i < VGA_WIDTH; i++) {
    vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + i] = vga_char(' ', color);
  }
}

void vga_putchar(char c) {
  switch (c) {
  case '\n':
    col = 0;
    if (row < VGA_HEIGHT - 1)
      row++;
    else
      scroll();
    vga_update_cursor(get_cursor());
    break;
  case '\r':
    col = 0;
    vga_update_cursor(get_cursor());
    break;
  case '\b':
    if (col > 0)
      col -= 1;
    else {
      col = VGA_WIDTH - 1;
      row--;
    }
    vga_buffer[get_cursor()] = vga_char(' ', color);
    vga_update_cursor(get_cursor());
    break;
  case '\t':
    col = ((col / TAB_SIZE) + 1) * TAB_SIZE;
    if (col >= VGA_WIDTH) {
      col = 0;
      if (row < VGA_HEIGHT - 1)
        row++;
      else
        scroll();
    }
    vga_update_cursor(get_cursor());
    break;
  default:
    vga_buffer[get_cursor()] = vga_char(c, color);
    update_cursor();

    break;
  }
}

void vga_set_cursor(size_t r, size_t c) {
  if (r >= VGA_HEIGHT || c >= VGA_WIDTH)
    return;
  row = r;
  col = c;
  vga_update_cursor(get_cursor());
}

void vga_get_cursor(size_t *r, size_t *c) {
  if (r)
    *r = row;
  if (c)
    *c = col;
}