
#ifndef VMM_H
#define VMM_H
#include <stdint.h>

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

// ? https://wiki.osdev.org/X86_Paging

typedef uint32_t pde_t;
typedef uint32_t pte_t;

typedef struct {
  pde_t entries[PAGE_ENTRIES];
} page_directory_t;

typedef struct {
  pte_t entries[PAGE_ENTRIES];
} page_table_t;

void vmm_init(void);
uint32_t vmm_get_physical_addr(uint32_t virtual_addr);
void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void vmm_unmap_page(uint32_t virtual_addr);
uint32_t vmm_alloc_user_page(void);
uint32_t vmm_alloc_kernel_page(void);

#endif