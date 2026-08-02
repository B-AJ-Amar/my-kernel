
#ifndef PIT_H
#define PIT_H

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_COMMAND 0x43
#define PIT_DEFAULT_FREQUENCY 1193182
#define PIT_FREQUENCY 100
// divisor = PIT_DEFAULT_FREQUENCY / PIT_FREQUENCY
#include <stdint.h>

/*
0x36 = 00 11 011 0
00 = channel 0
11 = access mode: lobyte/hibyte
011 = operating mode: 3 (square wave generator)
*/

/*
pit -> 1193182 ticks per second (if the divisor is 1)
if i want to make it 100 ticks per second, i need to divide it by 1193182 / 100
= 11931.82 e.g with devisor =  5

----x----x----x...x----(1sec)

*/

void pit_init(uint32_t frequency);
void pit_interrupt_handler(void);
void pit_wait(uint32_t ms);
void pit_wait_sec(uint32_t sec);

#endif
