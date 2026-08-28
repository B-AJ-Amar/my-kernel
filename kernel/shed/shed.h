#ifndef SHED_H
#define SHED_H

#include <stdint.h>

typedef enum {
  PROCESS_READY,
  PROCESS_RUNNING,
  PROCESS_BLOCKED
} process_state_t;

typedef struct thread_t {
  uint32_t tid;

  process_state_t state;

  uintptr_t stack;

  struct thread_t *next;
} thread_t;

void shed_init(void);
void task_create(void(*task), void *params);
void task_delay(uint64_t ms);
int task_exit(int code);
#endif