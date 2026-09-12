
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

#define KERNEL_STACK_ADDR 0xFFBFFFFF // pd[1022]
#define KERNEL_STACK_SIZE 0x4000 // 16kb (4 pages)
#define KERNEL_STACK_SIZE_PAGES (KERNEL_STACK_SIZE / PAGE_SIZE)

#define VMM_RECURSIVE_INDEX 1023
#define VMM_PD_VIRTUAL_ADDR  0xFFFFF000
#define VMM_PT_VIRTUAL_ADDR(dir_idx) (0xFFC00000 | ((uint32_t)(dir_idx) << 12))

// ? HEAP
#define KERNEL_HEAP_START 0xD0000000
#define KERNEL_HEAP_END 0xE0000000
#define KERNEL_HEAP_SIZE (KERNEL_HEAP_END - KERNEL_HEAP_START) // 256mb

/* ============================================================
 * VMM
 * ============================================================ */

#define PAGE_F_PRESENT        (1U << 0)
#define PAGE_F_WRITABLE       (1U << 1)
#define PAGE_F_USER           (1U << 2)
#define PAGE_F_WRITE_THROUGH  (1U << 3)
#define PAGE_F_CACHE_DISABLE  (1U << 4)
#define PAGE_F_ACCESSED       (1U << 5)
#define PAGE_F_DIRTY          (1U << 6)
#define PAGE_F_SIZE           (1U << 7)
#define PAGE_F_GLOBAL         (1U << 8)

#define PAGE_ENTRIES 1024
#define PAGE_SIZE    4096

/*
 * Virtual address layout
 *
 * 0x00000000 - 0xBFFFFFFF
 *      User space
 *
 * 0xC0000000 - 0xFFFFFFFF
 *      Kernel space
 *
 * PDE 1022:
 *      0xFF800000 - 0xFFBFFFFF
 *      Kernel stack region
 *
 * PDE 1023:
 *      0xFFC00000 - 0xFFFFFFFF
 *      Recursive paging
 */

/* User space */
#define USER_SPACE_BASE 0x00001000
#define USER_SPACE_END  0xBFFFFFFF
#define USER_SPACE_SIZE (USER_SPACE_END - USER_SPACE_BASE + 1)

/* Kernel space */
#define KERNEL_SPACE_BASE 0xC0000000
#define KERNEL_SPACE_END  0xFFFFFFFF
#define KERNEL_SPACE_SIZE (KERNEL_SPACE_END - KERNEL_SPACE_BASE + 1)

/* Kernel heap */
#define KERNEL_HEAP_START 0xD0000000
#define KERNEL_HEAP_END   0xE0000000
#define KERNEL_HEAP_SIZE  (KERNEL_HEAP_END - KERNEL_HEAP_START)

/*
 * Recursive paging
 *
 * PDE[1023] points to the page directory itself.
 *
 * 0xFFC00000 -> page table 0
 * 0xFFC01000 -> page table 1
 * ...
 * 0xFFFFE000 -> page table 1022
 * 0xFFFFF000 -> page directory
 */
#define RECURSIVE_PDE_INDEX 1023

#define RECURSIVE_PT_BASE 0xFFC00000
#define RECURSIVE_PD_ADDR 0xFFFFF000

/*
 * Kernel stack
 *
 * PDE[1022] covers:
 *
 * 0xFF800000 - 0xFFBFFFFF
 */
#define KERNEL_STACK_PDE_INDEX 1022

#define KERNEL_STACK_ADDR 0xFFBFFFFF
#define KERNEL_STACK_SIZE 0x4000
#define KERNEL_STACK_SIZE_PAGES (KERNEL_STACK_SIZE / PAGE_SIZE)


void mm_init(boot_info_t *boot,uintptr_t sp_top,uintptr_t sp_bottom);
#endif

