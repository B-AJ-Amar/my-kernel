#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/keyboard/ps2.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// phisical key -> protocol (e.g scan_set2) -> logical key (e.g KEY_Q) -> layout
// (e.g 'a' | 'A' | 'q' | 'Q')

static keyboard_layout_t *current_layout;
static keyboard_state_t current_state = {0};
static keyboard_driver_t *current_driver;
// todo: add available layouts array to make crtl + alt to change the layouts
void init_keyboard(kb_backend_t backend, const keyboard_layout_t *layout) {
  current_state.shift = false;
  current_state.ctrl = false;
  current_state.alt = false;
  current_state.caps_lock = false;

  current_layout = layout;
  // todo: add more backends
  if (backend == KB_BACKEND_PS2) {
    current_driver = &ps2_keyboard_driver;
  } else {
    panic("Unsupported keyboard backend: %d\n", backend);
  }
  current_driver->init();
}

keyboard_layout_t *keyboard_get_layout(void) { return current_layout; }

void keyboard_set_layout(const keyboard_layout_t *layout) {
  current_layout = layout;
}

bool is_printable_key(uint8_t scancode) {
  if (current_layout->keys_lower[scancode] != 0) {
    return true;
  }
  return false;
}

keyboard_driver_t *keyboard_get_driver(void) { return current_driver; }