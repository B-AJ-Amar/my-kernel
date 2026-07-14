#ifndef DRIVER_KEYBOARD_PS2_H
#define DRIVER_KEYBOARD_PS2_H
#include <drivers/keyboard/keyboard.h>
#include <stdbool.h>

#define KB_CMD_IDENTIFY 0xF2
#define KB_CMD_ENABLE_SCAN 0xF4
#define KB_CMD_RESEND 0xFE
#define KB_CMD_RESET 0xFF

#define KB_RES_ACK 0xFA
#define KB_RES_RESEND 0xFE
#define KB_RES_SELF_TEST_PASSED 0xAA
#define MAX_RETRIES 3

extern keyboard_driver_t ps2_keyboard_driver;

bool ps2_keyboard_init(void);
void ps2_keyboard_handle_interupt(void);

#endif