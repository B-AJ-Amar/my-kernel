#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/keyboard/ps2.h>
#include <stdbool.h>
#include <stdint.h>

static const keyboard_layout_t *current_layout;
static keyboard_state_t current_state = {0};
// todo: add available layouts array to make crtl + alt to change the layouts
void init_keyboard(kb_backend_t backend, const keyboard_layout_t *layout) {
  current_state.shift = false;
  current_state.ctrl = false;
  current_state.alt = false;
  current_state.caps_lock = false;

  current_layout = layout;
}

const keyboard_layout_t *keyboard_get_layout(void) { return current_layout; }

void keyboard_set_layout(const keyboard_layout_t *layout) {
  current_layout = layout;
}

bool is_printable_key(uint8_t scancode) {
  if (current_layout->keys_lower[scancode] != 0) {
    return true;
  }
  return false;
}