

#include <asm.h>
#include <kernel/shed/shed.h>
#include <mm/heap/heap.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// TODO: make asm.c file , and make asm function that use intel syntax
static void save_context(thread_t *thread) {
  //    pushad();
  //    thread->stack = (uintptr_t)asm("mov %%esp, %0" : "=r"(thread->stack));
  //    popad();
}

// static void restore_context(thread_t *thread) {
//    asm("mov %0, %%esp" : : "r"(thread->stack));
//    popad();
// }

static thread_t *tasks = NULL;
static uint32_t current_tid = 0;

void shed_init(void) {
  thread_t *task = kmalloc(sizeof(thread_t));
  task->state = PROCESS_RUNNING;
  task->tid = current_tid++;
}

// change between
void task_create(void(*task), void *params) {}
void task_sleep(uint64_t ms);