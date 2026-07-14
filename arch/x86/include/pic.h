
#ifndef PIC_H
#define PIC_H

#include <stdint.h>

#define PIC1 0x20
#define PIC2 0xA0

#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

/*
d4 init bit = 1
d3 Level Trigger Mode
    0 = Edge Triggered
    1 = Level Triggered
d2: not needed for x86
d1 cascade = 1 | single = 0
d0 need ICW4 ?

*/
#define PIC_ICW1_INIT 0b1001
/*
d3: Special Fully Nested Mode ? (used in case of multiple slaves)
d2: buffered mode (does it conneced to an external buffer ?)
d1: audo End of interupt ?
d0: mode  (protocol)
    0 = 8086/88  mode
    1 = MCS-80/85 mode

*/
#define IWC4_DATA 0x01

#define PIC_ALLOW_ALL 0x00
#define PIC_MASK_ALL 0xFF
#define PIC_ALLOW_KEYBOARD 0xFD

#define PIC_EOI 0x20

extern void pic_send_eoi(uint8_t irq);

void pic_send_command(uint8_t pic, uint8_t cmd);

void pic_send_data(uint8_t pic, uint8_t data);

uint8_t pic_read_mask(uint8_t pic);

void pic_write_mask(uint8_t pic, uint8_t mask);

void pic_init(void);

#endif