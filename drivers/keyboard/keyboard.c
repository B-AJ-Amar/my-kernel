#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/layout.h>
#include <drivers/keyboard/ps2.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// phisical key -> protocol (e.g scan_set2 (keymap.c)) -> logical key (e.g
// KEY_Q) -> layout (e.g 'a' | 'A' | 'q' | 'Q')

static char *keyboard_layout_get_current_keys();

static keyboard_layout_t *current_layout;
static keyboard_state_t current_state = {0};
static keyboard_driver_t *current_driver;
static keyboard_event_queue event_queue;
// todo: add available layouts array to make crtl + alt to change the layouts
void init_keyboard(kb_backend_t backend, const keyboard_layout_t *layout) {
  current_state.left_shift = false;
  current_state.right_shift = false;
  current_state.left_ctrl = false;
  current_state.right_ctrl = false;
  current_state.left_alt = false;
  current_state.right_alt = false;
  current_state.caps_lock = false;

  current_layout = layout;

  event_queue.head = 0;
  event_queue.tail = 0;
  event_queue.capacity = KB_EVENT_Q_SIZE;

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

bool is_printable_key(uint8_t key_code) {

  if (keyboard_layout_get_current_keys()[key_code] != 0) {
    return true;
  }
  return false;
}

keyboard_driver_t *keyboard_get_driver(void) { return current_driver; }

bool keyboard_even_is_empty(keyboard_event_queue *queue) {
  if (queue->head == queue->tail)
    return true;

  return false;
}

bool keyboard_even_is_full(keyboard_event_queue *queue) {
  if ((queue->tail + 1) % queue->capacity == queue->head)
    return true;

  return false;
}

bool keyboard_push_event(keyboard_event_t event) {
  if (keyboard_even_is_full(&event_queue)) {
    return false;
  }
  event_queue.buffer[event_queue.tail] = event;
  event_queue.tail = (event_queue.tail + 1) % event_queue.capacity;
  return true;
}

bool keyboard_pop_event(keyboard_event_t *event) {
  if (keyboard_even_is_empty(&event_queue)) {
    return false;
  }
  *event = event_queue.buffer[event_queue.head];
  event_queue.head = (event_queue.head + 1) % event_queue.capacity;
  return true;
}

keyboard_event_t *keyboard_peek_event(keyboard_event_t *event) {
  if (keyboard_even_is_empty(&event_queue)) {
    return NULL;
  }
  return &event_queue.buffer[event_queue.head];
}

bool keyboard_shift(void) {
  return current_state.left_shift || current_state.right_shift;
}

bool keyboard_ctrl(void) {
  return current_state.left_ctrl || current_state.right_ctrl;
}

bool keyboard_alt(void) {
  return current_state.left_alt || current_state.right_alt;
}

bool keyboard_caps_lock(void) { return current_state.caps_lock; }

static char *keyboard_layout_get_current_keys() {
  if (keyboard_shift() || keyboard_caps_lock()) {
    return current_layout->keys_upper;
  } else {
    return current_layout->keys_lower;
  }
}

void keyboard_handle_event(uint8_t key_code, keyboard_event_type_t type) {
  keyboard_event_t event;
  event.type = type;
  event.key = key_code;
  event.character = 0;
  if (is_printable_key(key_code)) {
    event.character = keyboard_layout_get_current_keys()[key_code];
  } else {
    switch (key_code) {
    case LEFT_SHIFT_KEY:
      current_state.left_shift = (type == KEY_PRESS);
      break;
    case RIGHT_SHIFT_KEY:
      current_state.right_shift = (type == KEY_PRESS);
      break;
    case LEFT_CTRL_KEY:
      current_state.left_ctrl = (type == KEY_PRESS);
      break;
    case RIGHT_CTRL_KEY:
      current_state.right_ctrl = (type == KEY_PRESS);
      break;
    case LEFT_ALT_KEY:
      current_state.left_alt = (type == KEY_PRESS);
      break;
    case RIGHT_ALT_KEY:
      current_state.right_alt = (type == KEY_PRESS);
      break;
    case CAPS_LOCK_KEY:
      if (type == KEY_PRESS) {
        current_state.caps_lock = !current_state.caps_lock;
      }
      break;
    default:
      break;
    }
  }
  keyboard_push_event(event);
}
