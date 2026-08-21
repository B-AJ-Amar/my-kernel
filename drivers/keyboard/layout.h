#ifndef DRIVER_KEYBOARD_LAYOUT_H
#define DRIVER_KEYBOARD_LAYOUT_H
#define KB_LAYOUT_SIZE 256

typedef struct {
  char *name;
  char keys_upper[KB_LAYOUT_SIZE];
  char keys_lower[KB_LAYOUT_SIZE];
} keyboard_layout_t;

extern keyboard_layout_t layout_us;
extern keyboard_layout_t layout_fr;

#endif