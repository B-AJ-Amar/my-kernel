#ifndef SHED_H
#define SHED_H

#include <stdint.h>

#define THREAD_READY   0
#define THREAD_RUNNING 1
#define THREAD_BLOCKED 2

#define SHED_CONTEXT_TIME 100  

typedef struct thread_t {
    uint32_t sp;                // saved ESP (points to interrupt frame)
    uint32_t tid;
    uint8_t  state;
    struct thread_t *next;
    struct thread_t *prev;
} thread_t;

extern void switch_context(uint32_t *old_esp, uint32_t *new_esp);
extern void switch_context_interrupt(uint32_t *old_esp, uint32_t *new_esp);
extern void task_stub(void);    // initial EIP for new tasks

void      shed_init(void);
thread_t *task_create(void (*entry)(void *), void *params);
void      schedule(void);
void      task_exit(int code) __attribute__((noreturn));
thread_t *task_get_current(void);

void task_wrapper(void (*entry)(void *), void *params);

#endif