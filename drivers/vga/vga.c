#include <drivers/vga.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
// TODO:add specal chars support (e.g \n \t ...)
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
static inline uint16_t get_cursor(void);
static inline uint16_t vga_char(char character, uint8_t color);

void vga_set_color(uint8_t fg, uint8_t bg) { color = bg << 4 | fg; }

void vga_reset_color(void) { color = VGA_COLOR_BLACK << 4 | VGA_COLOR_WHITE; }

void vga_clear(void) {
  vga_reset_color();
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_putchar(" ");
  }
}

uint16_t vga_char(char character, uint8_t color) {
  return ((uint16_t)color << 8) | (uint8_t)character;
}

static void update_cursor(void) {
  if (col < VGA_WIDTH)
    col++;
  else {
    col = 0;
    if (row < VGA_WIDTH)
      row++;
    // TODO: add scroll()
  }
}

static uint16_t get_cursor(void) { return row * VGA_WIDTH + col; }

void vga_putchar(char c) {
  vga_buffer[get_cursor()] = vga_char(c, color);
  update_cursor();
}

void vga_write(const char *str) {
  while (*str)
    vga_putchar(*str++);
}