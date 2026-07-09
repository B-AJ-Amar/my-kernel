#ifndef PS2_H
#define PS2_H
// https://wiki.osdev.org/I8042_PS/2_Controller
#include <stdint.h>

#define PS2_DATA_PORT 0x60
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64

#define PS2_SR_BUFFER_FULL (1 << 0)
#define PS2_SR_INPUT_FULL (1 << 1)
#define PS2_SR_SYSTEM (1 << 2)
#define PS2_SR_CMD_DATA (1 << 3)
#define PS2_SR_TIMEOUT (1 << 6)
#define PS2_SR_PARITY (1 << 7)

#define PS2_CMD_READ_CONFIG 0x20
#define PS2_CMD_WRITE_CONFIG 0x60
#define PS2_CMD_SELF_TEST 0xAA
#define PS2_CMD_ENABLE_PORT1 0xAE
#define PS2_CMD_DISABLE_PORT1 0xAD
#define PS2_CMD_TEST_PORT1 0xAB
#define PS2_CMD_ENABLE_PORT2 0xA8
#define PS2_CMD_DISABLE_PORT2 0xA7
#define PS2_CMD_TEST_PORT2 0xA9

#define PS2_RES_SELF_TEST_SUCCESS 0x55
#define PS2_RES_PORT_TEST_SUCCESS 0x00

#define PS2_CFG_PORT1_INT (1 << 0)
#define PS2_CFG_PORT2_INT (1 << 1)
#define PS2_CFG_SYSTEM (1 << 2)
// should be 0
#define PS2_CFG_PORT1_CLOCK (1 << 4)
#define PS2_CFG_PORT2_CLOCK (1 << 5)
#define PS2_CFG_PORT1_TRANSLATION (1 << 6)
// must be 0

void ps2_init(void);
void ps2_write_data(uint8_t data);
void ps2_write_command(uint8_t cmd);
uint8_t ps2_read_data(void);
uint8_t ps2_read_status(void);
void ps2_wait_for_input(void);
void ps2_wait_for_output(void);
void flush_buffer(void);
#endif