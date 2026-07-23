#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/keymap.h>
#include <drivers/keyboard/ps2.h>
#include <drivers/ps2/controller.h>
#include <kernel/console.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static bool __extended = false;
static bool __released = false;

keyboard_driver_t ps2_keyboard_driver = {.init = ps2_keyboard_init,
                                         .interupt_handler =
                                             ps2_keyboard_handle_interupt,
                                         .set_layout = NULL,
                                         .set_state = NULL};
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
  if (scancode == KB_SET2_EXTENDED) {
    __extended = true;
    return;
  } else if (scancode == KB_SET2_RELEASED) {
    __released = true;
    return;
  }

  keyboard_handle_event(keyboard_ps2_set2_decode(scancode, __extended),
                        __released ? KEY_RELEASE : KEY_PRESS);
  __extended = false;
  __released = false;
}
