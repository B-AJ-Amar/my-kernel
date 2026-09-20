#include <asm.h>
#include <interupts/interupt.h>
#include <kernel/shed/shed.h>
#include <mm/heap/heap.h>
#include <mm/mm.h>
#include <mm/vmm/vmm.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
// TODO: add a locking mechanism on create task to avoid interupts and race
// conditions
static thread_t *tasks = NULL;
static uint32_t next_tid = 0;
static thread_t *current_task = NULL;
static uintptr_t current_stack_offset = KERNEL_STACK_ADDR - KERNEL_STACK_SIZE;

static void task_insert(thread_t *current, thread_t *task);
static int task_delete(thread_t *task);
static uintptr_t task_alloc_stack(void);

void shed_init(void) {
  thread_t *task = kmalloc(sizeof(thread_t));
  task->tid = next_tid++;
  task->state = THREAD_RUNNING;
  task->sp = 0; /* set on the first context switch */
  task->next = NULL;
  task->prev = NULL;
  tasks = task;
  current_task = task;
}

thread_t *task_create(void (*entry)(void *), void *params) {
  thread_t *task = kmalloc(sizeof(thread_t));
  task->tid = next_tid++;
  task->state = THREAD_READY;

  uint32_t stack_top = task_alloc_stack();

  // ? 52 bytes = 8 pushad + 2 int_no/err_code + 3 eip/cs/eflags
  uint32_t *frame = (uint32_t *)(stack_top - 52);

  frame[0] = 0;                    // edi
  frame[1] = 0;                    // esi
  frame[2] = 0;                    // ebp
  frame[3] = 0;                    // esp (ignored by popad)
  frame[4] = (uint32_t)params;     // ebx -> 2nd arg
  frame[5] = 0;                    // edx
  frame[6] = 0;                    // ecx
  frame[7] = (uint32_t)entry;      // eax -> 1st arg
  frame[8] = 0;                    // int_no
  frame[9] = 0;                    // err_code
  frame[10] = (uint32_t)task_stub; // eip
  frame[11] = CODE_SEG;            // cs
  frame[12] = DEFAULT_EFLAGS;      // eflags

  task->sp = (uint32_t)frame;

  task_insert(current_task, task);
  return task;
}

void schedule(interupt_registers_t *regs) {
  thread_t *next = current_task->next;
  if (next == NULL)
    next = tasks;
  if (next == current_task)
    return;

  current_task->sp = (uint32_t)regs;

  if (current_task->state == THREAD_RUNNING)
    current_task->state = THREAD_READY;
  next->state = THREAD_RUNNING;

  current_task = next;

  switch_to_task(next->sp);
  __builtin_unreachable();
}

__attribute__((noreturn)) void task_exit(int code) {
  (void)code;
  __block_thread__();
}

__attribute__((noreturn)) void __block_thread__(void) {
  if (current_task == NULL)
    panic("__block_thread__: no current task");

  thread_t *next = current_task->next;
  if (next == NULL)
    next = tasks;
  if (next == current_task)
    panic("__block_thread__: no runnable tasks");

  task_delete(current_task);
  next->state = THREAD_RUNNING;
  current_task = next;

  switch_to_task(next->sp);
  __builtin_unreachable();
}

static void task_insert(thread_t *current, thread_t *task) {
  if (tasks == NULL) {
    task->next = NULL;
    task->prev = NULL;
    tasks = task;
    return;
  }
  task->next = current->next;
  task->prev = current;
  if (current->next)
    current->next->prev = task;
  current->next = task;
}

static int task_delete(thread_t *task) {
  if (task == NULL)
    return -1;
  if (task->prev)
    task->prev->next = task->next;
  else
    tasks = task->next;
  if (task->next)
    task->next->prev = task->prev;
  kfree(task);
  return 0;
}

static uintptr_t task_alloc_stack(void) {
  uintptr_t stack_addr = current_stack_offset;
  current_stack_offset -= KERNEL_STACK_SIZE;

  for (uint32_t i = 0; i < KERNEL_STACK_SIZE_PAGES; i++) {
    uintptr_t vaddr = stack_addr - (i + 1) * PAGE_SIZE;
    if (!vmm_test_if_mapped(vaddr)) {
      if (vmm_alloc_page(vaddr, PAGE_F_PRESENT | PAGE_F_WRITABLE) == 0)
        panic("Failed to allocate task stack page");
    }
  }
  return stack_addr;
}

void task_wrapper(void (*entry)(void *), void *params) {
  entry(params);
  task_exit(0);
}

thread_t *task_get_current(void) { return current_task; }