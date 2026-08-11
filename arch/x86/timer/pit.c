#include <io.h>
#include <timer/pit.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t pit_ticks, pit_frequency;

void pit_init(uint32_t frequency) {
  pit_ticks = 0;
  uint64_t divisor = PIT_DEFAULT_FREQUENCY / frequency;
  pit_frequency = frequency;

  //
  outb(PIT_COMMAND, 0x36);

  outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));        // low
  outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF)); // high
}

void pit_interrupt_handler(void) { pit_ticks++; }

void pit_wait(uint32_t ms) {
  uint64_t start_ticks = pit_ticks;
  uint64_t wait_ticks = (pit_frequency * ms) / 1000;

  while ((pit_ticks - start_ticks) < wait_ticks) {
  }; // todo: use scheduler to avoid busy waiting
}

void pit_wait_sec(uint32_t sec) { pit_wait(sec * 1000); }
