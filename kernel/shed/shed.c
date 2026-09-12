#include <asm.h>
#include <kernel/shed/shed.h>
#include <mm/heap/heap.h>
#include <mm/mm.h>
#include <mm/vmm/vmm.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#define CODE_SEG 0x08
#define DATA_SEG 0x10
#define DEFAULT_EFLAGS 0x202   // IF=1, bit 1 is always 1
static thread_t *tasks         = NULL;
static uint32_t  next_tid      = 0;
static thread_t *current_task  = NULL;
static uintptr_t current_stack_offset =
    KERNEL_STACK_ADDR - KERNEL_STACK_SIZE;   // reserve space for main kernel thread

static void task_insert(thread_t *current, thread_t *task);
static int  task_delete(thread_t *task);
static uintptr_t task_alloc_stack(void);

void shed_init(void) {
    thread_t *task = kmalloc(sizeof(thread_t));
    task->tid   = next_tid++;
    task->state = THREAD_RUNNING;
    task->next  = NULL;
    task->prev  = NULL;
    tasks        = task;
    current_task = task;
    /* sp will be filled on the first context switch */
}

thread_t *task_create(void (*entry)(void *), void *params) {
    thread_t *task = kmalloc(sizeof(thread_t));
    task->tid   = next_tid++;
    task->state = THREAD_READY;

    uint32_t stack_top = task_alloc_stack();
    /* Reserve space for the initial interrupt frame (17 dwords = 68 bytes) */
    uint32_t *frame = (uint32_t *)(stack_top - 17 * 4);

    /* Build the frame expected by switch_context_interrupt:
     *   gs, fs, es, ds,
     *   edi, esi, ebp, esp, ebx, edx, ecx, eax,
     *   int_no, err_code,
     *   eip, cs, eflags
     */
    frame[0]  = 0;                     // gs
    frame[1]  = 0;                     // fs
    frame[2]  = DATA_SEG;              // es
    frame[3]  = DATA_SEG;              // ds

    frame[4]  = 0;                     // edi
    frame[5]  = 0;                     // esi
    frame[6]  = 0;                     // ebp
    frame[7]  = 0;                     // esp (ignored by popad)
    frame[8]  = (uint32_t)params;      // ebx -> arg2 for task_wrapper
    frame[9]  = 0;                     // edx
    frame[10] = 0;                     // ecx
    frame[11] = (uint32_t)entry;       // eax -> arg1 for task_wrapper

    frame[12] = 0;                     // int_no
    frame[13] = 0;                     // err_code
    frame[14] = (uint32_t)task_stub;   // eip
    frame[15] = CODE_SEG;              // cs
    frame[16] = DEFAULT_EFLAGS;        // eflags

    task->sp = (uint32_t)frame;

    task_insert(current_task, task);
    return task;
}

void schedule(void) {
    thread_t *next = current_task->next;
    if (next == NULL) next = tasks;
    if (next == current_task) return;          // nothing else to run

    if (current_task->state == THREAD_RUNNING)
        current_task->state = THREAD_READY;
    next->state = THREAD_RUNNING;

    thread_t *prev = current_task;
    current_task   = next;

    switch_context_interrupt(&prev->sp, &next->sp);
    __builtin_unreachable();   
}

__attribute__((noreturn))
void task_exit(int code) {
    (void)code;
    if (current_task == NULL) panic("task_exit called with no current task");

    thread_t *next = current_task->next;
    if (next == NULL) next = tasks;
    if (next == current_task) panic("No more tasks to run");

    task_delete(current_task);
    next->state  = THREAD_RUNNING;
    current_task = next;

    switch_context_interrupt(NULL, &next->sp);
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
    if (current->next) current->next->prev = task;
    current->next = task;
}

static int task_delete(thread_t *task) {
    if (task == NULL) return -1;
    if (task->prev) task->prev->next = task->next;
    else            tasks            = task->next;
    if (task->next) task->next->prev = task->prev;
    kfree(task);
    return 0;
}

static uintptr_t task_alloc_stack(void) {
    uintptr_t stack_addr = current_stack_offset;
    current_stack_offset -= KERNEL_STACK_SIZE;
    printf("[\033[2,0]x\033[15,0]] Allocated stack for task at: 0x%lx\n", stack_addr);

    for (uint32_t i = 0; i < KERNEL_STACK_SIZE_PAGES; i++) {
        uintptr_t vaddr = stack_addr - (i + 1) * PAGE_SIZE;
        printf("  [\033[2,0]x\033[15,0]] Allocating stack page at: 0x%lx\n", vaddr);
        if (!vmm_test_if_mapped(vaddr)) {
            if (vmm_alloc_page(vaddr, PAGE_F_PRESENT | PAGE_F_WRITABLE) == 0)
                panic("Failed to allocate task stack page");
        }
    }
    return stack_addr;   // top of stack (highest address)
}

void task_wrapper(void (*entry)(void *), void *params) {
    entry(params);
    task_exit(0);
}

thread_t *task_get_current(void) {
    return current_task;
}