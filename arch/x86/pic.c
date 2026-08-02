#include <io.h>
#include <pic.h>
#include <stdio.h>
// 8259A pic dadasheet : https://pccomponents.com/datasheets/INTEL-P8259A2.pdf
// you will ICW config : datasheet pages 10,11,12

void pic_send_eoi(uint8_t irq) {
  if (irq >= 8)
    outb(PIC2_COMMAND, PIC_EOI);

  outb(PIC1_COMMAND, PIC_EOI);
}

void pic_send_command(uint8_t pic, uint8_t cmd) {
  if (pic == PIC1)
    outb(PIC1_COMMAND, cmd);
  else if (pic == PIC2)
    outb(PIC2_COMMAND, cmd);
  else
    panic("Invalid PIC number: %u\n", pic);
}

void pic_send_data(uint8_t pic, uint8_t data) {
  if (pic == PIC1)
    outb(PIC1_DATA, data);
  else if (pic == PIC2)
    outb(PIC2_DATA, data);
  else
    panic("Invalid PIC number: %u\n", pic);
}

uint8_t pic_read_mask(uint8_t pic) {
  if (pic == PIC1)
    return inb(PIC1_DATA);
  else if (pic == PIC2)
    return inb(PIC2_DATA);
  else
    panic("Invalid PIC number: %u\n", pic);
}

void pic_write_mask(uint8_t pic, uint8_t mask) {
  if (pic == PIC1)
    outb(PIC1_DATA, mask);
  else if (pic == PIC2)
    outb(PIC2_DATA, mask);
  else
    panic("Invalid PIC number: %u\n", pic);
}

void pic_init(void) {
  pic_write_mask(PIC1, PIC_MASK_ALL);
  pic_write_mask(PIC2, PIC_MASK_ALL);

  // ICW1
  pic_send_command(PIC1, PIC_ICW1_INIT);
  pic_send_command(PIC2, PIC_ICW1_INIT);

  // ICW2: remap offset address of IDT
  pic_send_data(PIC1, PIC1_OFFSET);
  pic_send_data(PIC2, PIC2_OFFSET);

  // ICW3: setup cascading
  pic_send_data(PIC1, 0x04); // there is a slave in  IRQ2 (00000100)
  pic_send_data(
      PIC2, 0x02); // tell Slave PIC connected to the master via the second pin
                   // (00000010)  (if its connected to the third => 00000011)

  // ICW4
  pic_send_data(PIC1, IWC4_DATA);
  pic_send_data(PIC2, IWC4_DATA);

  pic_write_mask(PIC1, PIC_ALLOW_KEYBOARD & PIC_ALLOW_TIMER);
  // pic_write_mask(PIC2, PIC_ALLOW_ALL  );
}
