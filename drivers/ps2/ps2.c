

#include <ps2.h>
#include <io.h>
#include <stdbool.h>
uint8_t ps2_read_status(void) {
  return inb(PS2_STATUS_PORT);
}

// todo: add a timout after timer driver
void ps2_wait_for_input(void) {
  while (ps2_read_status() & PS2_SR_INPUT_FULL)
    ;
}

void ps2_wait_for_output(void) {
  while (!(ps2_read_status() & PS2_SR_BUFFER_FULL))
    ;
}

void ps2_write_data(uint8_t data) {
  ps2_wait_for_input();
  outb(PS2_DATA_PORT, data);
}

void ps2_write_command(uint8_t cmd) {
  ps2_wait_for_input();
  outb(PS2_COMMAND_PORT, cmd);
}


uint8_t ps2_read(void) {
  ps2_wait_for_output();
  return inb(PS2_DATA_PORT);
}
// https://wiki.osdev.org/I8042_PS/2_Controller#Initialising_the_PS/2_Controller
void ps2_init(void)
{
    bool p1_available = false;
    bool p2_available = false;

    ps2_write_command(PS2_CMD_DISABLE_PORT1);
    ps2_write_command(PS2_CMD_DISABLE_PORT2);

    flush_buffer();

    ps2_write_command(PS2_CMD_READ_CONFIG);
    uint8_t config = ps2_read_data();

    // disable interrupts and translation
    config &= ~(PS2_CFG_PORT1_INT |
                PS2_CFG_PORT2_INT |
                PS2_CFG_PORT1_TRANSLATION);

    ps2_write_command(PS2_CMD_WRITE_CONFIG);
    ps2_write_data(config);

    ps2_write_command(PS2_CMD_SELF_TEST);

    uint8_t self_test_result = ps2_read_data();
    if (self_test_result != PS2_RES_SELF_TEST_SUCCESS) {
        panic("PS/2 controller self-test failed (0x%02X)", self_test_result);
    }


    ps2_write_command(PS2_CMD_ENABLE_PORT2);
    ps2_write_command(PS2_CMD_READ_CONFIG);
    config = ps2_read_data();
    p2_available = !(config & PS2_CFG_PORT2_CLOCK);
    ps2_write_command(PS2_CMD_DISABLE_PORT2);

    ps2_write_command(PS2_CMD_TEST_PORT1);
    uint8_t port1_result = ps2_read_data();
    if (port1_result == PS2_RES_PORT_TEST_SUCCESS)p1_available = true;
    else printf("PS/2 port 1 test failed \n");
    
    // test in the second intrface exits
    if (p2_available) {
        ps2_write_command(PS2_CMD_TEST_PORT2);

        uint8_t port2_result = ps2_read_data();

        if (port2_result == PS2_RES_PORT_TEST_SUCCESS) {
            p2_available = true;
        } else {
            p2_available = false;
            printf("PS/2 port 2 test failed \n");
        }
    }

    if (p1_available)
        ps2_write_command(PS2_CMD_ENABLE_PORT1);

    if (p2_available)
        ps2_write_command(PS2_CMD_ENABLE_PORT2);


    ps2_write_command(PS2_CMD_READ_CONFIG);
    config = ps2_read_data();
    if (p1_available)
        config |= PS2_CFG_PORT1_INT;
    if (p2_available)
        config |= PS2_CFG_PORT2_INT;
    ps2_write_command(PS2_CMD_WRITE_CONFIG);
    ps2_write_data(config);
    // reset will be donn by the drivers (keyboard and mouse)
}

static void flush_buffer(void)
{
    while (ps2_read_status() & PS2_SR_BUFFER_FULL)
        (void)ps2_read_data();
}