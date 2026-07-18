#ifndef DRIVER_KEYBOARD_H
#define DRIVER_KEYBOARD_H
#define KB_EVENT_Q_SIZE 256
// TODO: add keyboard_event and queue and tty
#include <drivers/keyboard/keymap.h>
#include <drivers/keyboard/layout.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct {
  bool left_shift;
  bool right_shift;

  bool left_ctrl;
  bool right_ctrl;

  bool left_alt;
  bool right_alt;

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
typedef enum { KEY_PRESS, KEY_RELEASE } keyboard_event_type_t;

typedef struct {
  keyboard_event_type_t type;
  keyboard_key_t key;
  uint32_t character;
} keyboard_event_t;

typedef struct {
  keyboard_event_t buffer[KB_EVENT_Q_SIZE];
  size_t head;
  size_t tail;
  size_t capacity;

} keyboard_event_queue;

void init_keyboard(kb_backend_t backend, const keyboard_layout_t *layout);
bool is_printable_key(uint8_t key_code);
keyboard_layout_t *keyboard_get_layout(void);
void keyboard_set_layout(const keyboard_layout_t *layout);
keyboard_driver_t *keyboard_get_driver(void);
// ? the protocol (ps2,usb ..) should know which key is presed/released ,this
// common function should handle the event and push it
void keyboard_handle_event(uint8_t key_code, keyboard_event_type_t type);

bool keyboard_even_is_empty(keyboard_event_queue *queue);
bool keyboard_even_is_full(keyboard_event_queue *queue);
bool keyboard_push_event(keyboard_event_t event);
bool keyboard_pop_event(keyboard_event_t *event);
keyboard_event_t *keyboard_peek_event(keyboard_event_t *event);

bool keyboard_shift(void);
bool keyboard_ctrl(void);
bool keyboard_alt(void);
bool keyboard_caps_lock(void);
#endif