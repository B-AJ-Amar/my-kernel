#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H
// TODO: add keyboard_event and queue and tty
#include <drivers/keyboard/layout.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct {
  bool shift;
  bool ctrl;
  bool alt;
  bool caps_lock;
} keyboard_state_t;

typedef enum {
  KB_BACKEND_PS2,
  KB_BACKEND_USB,
} kb_backend_t;
typedef struct keyboard_driver_t {
  bool (*init)(void);
  void (*interupt_handler)(void);
  void (*set_layout)(void);
  void (*set_state)(keyboard_state_t state);
} keyboard_driver_t;

void init_keyboard(kb_backend_t backend, const keyboard_layout_t *layout);
bool is_printable_key(uint8_t scancode);
keyboard_layout_t *keyboard_get_layout(void);
void keyboard_set_layout(const keyboard_layout_t *layout);
keyboard_driver_t *keyboard_get_driver(void);

#endif