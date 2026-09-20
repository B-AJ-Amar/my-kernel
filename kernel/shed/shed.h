#ifndef SHED_H
#define SHED_H

#include <interupts/interupt.h>
#include <stdint.h>

#define THREAD_READY 0
#define THREAD_RUNNING 1
#define THREAD_BLOCKED 2

#define SHED_CONTEXT_TIME 5 // each 5 ms

typedef struct thread_t {
  uint32_t sp; /* saved interrupt frame pointer (points at EDI) */
  uint32_t tid;
  uint8_t state;
  struct thread_t *next;
  struct thread_t *prev;
} thread_t;

extern void switch_to_task(uint32_t new_sp) __attribute__((noreturn));
extern void task_stub(void);

void shed_init(void);
thread_t *task_create(void (*entry)(void *), void *params);
void schedule(interupt_registers_t *regs);
void task_exit(int code) __attribute__((noreturn));
thread_t *task_get_current(void);
void __block_thread__(void) __attribute__((noreturn));

void task_wrapper(void (*entry)(void *), void *params);

#endif