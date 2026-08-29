#ifndef SHED_H
#define SHED_H

#define THREAD_READY 0
#define THREAD_RUNNING 1
#define THREAD_BLOCKED 2
#include <stdint.h>

typedef struct thread_t {
  uintptr_t sp;

  uint32_t tid;
  uint8_t state;

  struct thread_t *next;
  struct thread_t *prev;
} thread_t;

extern void switch_context(thread_t *current, thread_t *next);
void shed_init(void);
thread_t *task_create(void (*entry)(void *), void *params);
void task_delay(uint64_t ms);
int task_exit(int code);
#endif