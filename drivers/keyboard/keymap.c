#include <drivers/keyboard/keymap.h>
#include <stdbool.h>
// TODO: add protocol interface and add support for multiple protocols

static const keyboard_key_t ps2_set2_keymap[256];
static const keyboard_key_t ps2_set2_extended_keymap[256];

keyboard_key_t keyboard_ps2_set2_decode(uint8_t scancode, bool extended) {
  if (extended)
    return ps2_set2_extended_keymap[scancode];

  return ps2_set2_keymap[scancode];
}

static const keyboard_key_t ps2_set2_keymap[256] = {
    [0x01] = F9_KEY,
    [0x03] = F5_KEY,
    [0x04] = F3_KEY,
    [0x05] = F1_KEY,
    [0x06] = F2_KEY,
    [0x07] = F12_KEY,

    [0x09] = F10_KEY,
    [0x0A] = F8_KEY,
    [0x0B] = F6_KEY,
    [0x0C] = F4_KEY,
    [0x0D] = TAB_KEY,
    [0x0E] = BACKTICK_KEY,

    [0x11] = LEFT_ALT_KEY,
    [0x12] = LEFT_SHIFT_KEY,
    [0x14] = LEFT_CTRL_KEY,
    [0x15] = Q_KEY,
    [0x16] = NUM_1_KEY,

    [0x1A] = Z_KEY,
    [0x1B] = S_KEY,
    [0x1C] = A_KEY,
    [0x1D] = W_KEY,
    [0x1E] = NUM_2_KEY,

    [0x21] = C_KEY,
    [0x22] = X_KEY,
    [0x23] = D_KEY,
    [0x24] = E_KEY,
    [0x25] = NUM_4_KEY,
    [0x26] = NUM_3_KEY,

    [0x29] = SPACE_KEY,
    [0x2A] = V_KEY,
    [0x2B] = F_KEY,
    [0x2C] = T_KEY,
    [0x2D] = R_KEY,
    [0x2E] = NUM_5_KEY,

    [0x31] = N_KEY,
    [0x32] = B_KEY,
    [0x33] = H_KEY,
    [0x34] = G_KEY,
    [0x35] = Y_KEY,
    [0x36] = NUM_6_KEY,

    [0x3A] = M_KEY,
    [0x3B] = J_KEY,
    [0x3C] = U_KEY,
    [0x3D] = NUM_7_KEY,
    [0x3E] = NUM_8_KEY,

    [0x41] = COMMA_KEY,
    [0x42] = K_KEY,
    [0x43] = I_KEY,
    [0x44] = O_KEY,
    [0x45] = NUM_0_KEY,
    [0x46] = NUM_9_KEY,

    [0x49] = DOT_KEY,
    [0x4A] = SLASH_KEY,
    [0x4B] = L_KEY,
    [0x4C] = SEMICOLON_KEY,
    [0x4D] = P_KEY,
    [0x4E] = MINUS_KEY,

    [0x52] = SINGLE_QUOTE_KEY,
    [0x54] = LEFT_BRACKET_KEY,
    [0x55] = EQUALS_KEY,

    [0x58] = CAPS_LOCK_KEY,

    [0x59] = RIGHT_SHIFT_KEY,
    [0x5A] = ENTER_KEY,
    [0x5B] = RIGHT_BRACKET_KEY,
    [0x5D] = BACKSLASH_KEY,

    [0x66] = BACKSPACE_KEY,

    [0x69] = KEYPAD_1_KEY,
    [0x6B] = KEYPAD_4_KEY,
    [0x6C] = KEYPAD_7_KEY,

    [0x70] = KEYPAD_0_KEY,
    [0x71] = KEYPAD_DOT_KEY,
    [0x72] = KEYPAD_2_KEY,
    [0x73] = KEYPAD_5_KEY,
    [0x74] = KEYPAD_6_KEY,
    [0x75] = KEYPAD_8_KEY,
    [0x76] = ESC_KEY,

    [0x77] = NUM_LOCK_KEY,

    [0x79] = KEYPAD_PLUS_KEY,
    [0x7A] = KEYPAD_3_KEY,
    [0x7B] = KEYPAD_MINUS_KEY,
    [0x7C] = KEYPAD_MULTIPLY_KEY,
    [0x7D] = KEYPAD_9_KEY,

    [0x83] = F7_KEY,

    [0x84] = PRINT_SCREEN_KEY, // part of special sequence
};

static const keyboard_key_t ps2_set2_extended_keymap[256] = {

    /* Modifiers */

    [0x11] = RIGHT_ALT_KEY,
    [0x14] = RIGHT_CTRL_KEY,

    /* Keypad */

    [0x4A] = KEYPAD_DIVIDE_KEY,
    [0x5A] = KEYPAD_ENTER_KEY,

    /* Navigation */

    [0x69] = END_KEY,
    [0x6B] = LEFT_ARROW_KEY,
    [0x6C] = HOME_KEY,

    [0x70] = INSERT_KEY,
    [0x71] = DELETE_KEY,
    [0x72] = DOWN_ARROW_KEY,
    [0x74] = RIGHT_ARROW_KEY,
    [0x75] = UP_ARROW_KEY,

    [0x7A] = PAGE_DOWN_KEY,
    [0x7D] = PAGE_UP_KEY,

    /* Windows keys */

    [0x1F] = LEFT_GUI_KEY,
    [0x27] = RIGHT_GUI_KEY,
    [0x2F] = MENU_KEY,

    /* ACPI */

    // [0x37] = POWER_KEY,
    // [0x3F] = SLEEP_KEY,
    // [0x5E] = WAKE_KEY,

    // /* Multimedia */

    // [0x10] = WWW_SEARCH_KEY,
    // [0x15] = PREVIOUS_TRACK_KEY,
    // [0x1D] = WWW_FAVORITES_KEY,
    // [0x21] = VOLUME_DOWN_KEY,
    // [0x23] = MUTE_KEY,
    // [0x2B] = STOP_KEY,
    // [0x32] = VOLUME_UP_KEY,
    // [0x34] = PLAY_PAUSE_KEY,
    // [0x3A] = WWW_HOME_KEY,
    // [0x3B] = NEXT_TRACK_KEY,
    // [0x4D] = MEDIA_SELECT_KEY,
    // [0x50] = MAIL_KEY,
    // [0x4B] = CALCULATOR_KEY,
    // [0x48] = MY_COMPUTER_KEY,
    // [0x40] = WWW_FORWARD_KEY,
    // [0x38] = WWW_REFRESH_KEY,
    // [0x30] = WWW_STOP_KEY,
    // [0x28] = WWW_BACK_KEY,
};