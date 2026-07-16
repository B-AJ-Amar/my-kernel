#include <drivers/keyboard/keyboard.h>
#include <drivers/keyboard/keymap.h>
#include <drivers/keyboard/ps2.h>
#include <drivers/ps2/controller.h>
#include <kernel/console.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void ps2_handle_keycode(uint8_t scancode);

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
  keyboard_layout_t *current_layout = keyboard_get_layout();

  ps2_handle_keycode(scancode);
}

// TODO: add events and queue add release event and fix the keyboard handling
void ps2_handle_keycode(uint8_t scancode) {
  keyboard_layout_t *current_layout = keyboard_get_layout();
  if (current_layout != NULL &&
      current_layout->keys_lower[scan_set2_decode(scancode)] != 0) {
    printf("%c", current_layout->keys_lower[scan_set2_decode(scancode)]);
  } else
    switch (scan_set2_decode(scancode)) {
    case ENTER_KEY:
      printf("\n");
      break;
    case BACKSPACE_KEY:
      printf("\b \b");
      break;
    case TAB_KEY:
      printf("\t");
      break;
    case ESC_KEY:
      printf("ESC");
      break;
    case LEFT_SHIFT_KEY:
    case RIGHT_SHIFT_KEY:
      printf("SHIFT");
      break;
    case LEFT_CTRL_KEY:
    case RIGHT_CTRL_KEY:
      printf("CTRL");
      break;
    case LEFT_ALT_KEY:
    case RIGHT_ALT_KEY:
      printf("ALT");
      break;

    case CAPS_LOCK_KEY:
      printf("CAPS LOCK");
      break;

    case NUM_LOCK_KEY:
      printf("NUM LOCK");
      break;
    case SCROLL_LOCK_KEY:
      printf("SCROLL LOCK");
      break;

    case F1_KEY:
    case F2_KEY:
    case F3_KEY:
    case F4_KEY:
    case F5_KEY:
    case F6_KEY:
    case F7_KEY:
    case F8_KEY:
    case F9_KEY:
    case F10_KEY:
    case F11_KEY:
    case F12_KEY:
      printf("FX_kEY");
      break;

    case UP_ARROW_KEY:
      printf("UP");
      break;
    case DOWN_ARROW_KEY:
      printf("DOWN");
      break;
    case LEFT_ARROW_KEY:
      printf("LEFT");
      break;
    case RIGHT_ARROW_KEY:
      printf("RIGHT");
      break;

    case INSERT_KEY:
      printf("INSERT");
      break;
    case DELETE_KEY:
      printf("DELETE");
      break;
    case HOME_KEY:
      printf("HOME");
      break;
    case END_KEY:
      printf("END");
      break;
    case PAGE_UP_KEY:
      printf("PAGE UP");
      break;
    case PAGE_DOWN_KEY:
      printf("PAGE DOWN");
      break;

    default:
      break;
    }
}