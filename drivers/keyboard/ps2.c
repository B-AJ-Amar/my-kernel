#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/ps2.h>
#include <drivers/ps2/controller.h>
#include <kernel/console.h>
#include <stdbool.h>
#include <stdint.h>

void ps2_handle_keycode(uint8_t scancode);

keyboard_driver_t ps2_keyboard_driver = {
    .init = ps2_keyboard_init,
    .interupt_handler = ps2_keyboard_handle_interupt,
};
bool ps2_keyboard_init(void) {
  ps2_write_command(KB_CMD_RESET);
  uint8_t res = 0;
  for (int i = 0; i < MAX_RETRIES; i++) {
    res = ps2_read_data();
    if (res == KB_RES_RESEND)
      continue;
    if (res != KB_RES_ACK)
      return false;
    res = ps2_read_data();
    if (res != KB_RES_SELF_TEST_PASSED)
      return false;

    return true;
  }
  return false;
}

void ps2_keyboard_handle_interupt(void) {
  uint8_t scancode = ps2_read_data();

  ps2_handle_keycode(scancode);
}

// TODO: add events and queue
void ps2_handle_keycode(uint8_t scancode) {
  if (is_printable_key(scancode)) {
    char c = keyboard_get_layout()->keys_lower[scancode];
    console_putchar(c);
  }
}