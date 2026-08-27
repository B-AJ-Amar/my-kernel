
#ifndef MM_H
#define MM_H
#include <mm/pmm/boot_info.h>
#include <stdint.h>

// ? VMM
#define PAGE_F_PRESENT (1U << 0)
#define PAGE_F_WRITABLE (1U << 1)
#define PAGE_F_USER (1U << 2)
#define PAGE_F_WRITE_THROUGH (1U << 3)
#define PAGE_F_CACHE_DISABLE (1U << 4)
#define PAGE_F_ACCESSED (1U << 5)
#define PAGE_F_DIRTY (1U << 6)
#define PAGE_F_SIZE (1U << 7)
#define PAGE_F_GLOBAL (1U << 8)
#define PAGE_ENTRIES 1024
#define PAGE_SIZE 4096

#define KERNEL_SPACE_BASE 0xC0000000
#define KERNEL_SPACE_END 0xFFFFFFFF
#define KERNEL_SPACE_SIZE (KERNEL_SPACE_END - KERNEL_SPACE_BASE + 1)
#define USER_SPACE_BASE 0x00001000
#define USER_SPACE_END 0xBFFFFFFF
#define USER_SPACE_SIZE (USER_SPACE_END - USER_SPACE_BASE + 1)

#define KERNEL_STACK_ADDR 0xE0000000
#define KERNEL_STACK_SIZE 0x1000

// ? HEAP
#define KERNEL_HEAP_START 0xD0000000
#define KERNEL_HEAP_END   KERNEL_STACK_ADDR
#define KERNEL_HEAP_SIZE (KERNEL_HEAP_END - KERNEL_HEAP_START) // 256mb

void init_mm(boot_info_t *boot);
#endif