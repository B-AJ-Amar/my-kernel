

#include <asm.h>
#include <kernel/shed/shed.h>
#include <mm/heap/heap.h>
#include <mm/vmm/vmm.h>
#include <mm/mm.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static thread_t *tasks = NULL;
static uint32_t next_tid = 0;
static thread_t *current_task = NULL;
static uintptr_t current_stack_offset = KERNEL_STACK_ADDR - KERNEL_STACK_SIZE; // reserv the first part for the main kernel thread

static void task_insert(thread_t *current, thread_t *task);
static uintptr_t task_alloc_stack();

void shed_init(void)
{
    thread_t *task = kmalloc(sizeof(thread_t));

    task->tid = next_tid++;
    task->state = THREAD_RUNNING;
    task->next = NULL;
    task->prev = NULL;

    tasks = task;
    current_task = task;
}

thread_t *task_create(void (*entry)(void *), void *params)
{
    thread_t *task = kmalloc(sizeof(thread_t));

    task->tid = next_tid++;
    task->state = THREAD_READY;

    task->sp = task_alloc_stack();

    task_insert(current_task, task);

    return task;
}

void task_sleep(uint64_t ms);

// TODO: use thread_blocked for optimization
void schedule(void)
{
    thread_t *next = current_task->next;

    if (next == NULL)
        next = tasks;

    if (next == current_task)
        return;

    if (current_task->state == THREAD_RUNNING)
        current_task->state = THREAD_READY;

    next->state = THREAD_RUNNING;

    thread_t *prev = current_task;
    current_task = next;

    switch_context(prev, next);
}


void task_insert(thread_t *current,thread_t *task)
{
    if (tasks == NULL) {
        task->next = NULL;
        task->prev = NULL;
        tasks = task;
        return;
    }
    task->next = current->next;
    task->prev = current;

    current->next->prev = task;
    current->next = task;
}

uintptr_t task_alloc_stack(){
    
    uintptr_t stack_addr = current_stack_offset;
    current_stack_offset -= KERNEL_STACK_SIZE;
    if (!vmm_test_if_mapped(stack_addr)) {
        uint32_t vaddr = vmm_alloc_pages(current_stack_offset+1,stack_addr, KERNEL_STACK_SIZE_PAGES, PAGE_F_PRESENT | PAGE_F_WRITABLE);
        if (vaddr == 0) 
            panic("Failed to allocate stack for new task");
        
    }
    //TODO add the interupt pushed things to the stackstack things (IP,FLAGS,CS,SS,DS,ES,FS,GS) to the stack
    // return stack - pushed things
    return stack_addr;
}