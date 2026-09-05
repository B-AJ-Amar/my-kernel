
#ifndef VMM_H
#define VMM_H
#include <mm/mm.h>
#include <stdbool.h>
#include <stdint.h>
// ? https://wiki.osdev.org/X86_Paging

typedef uint32_t pde_t;
typedef uint32_t pte_t;

typedef struct {
  pde_t entries[PAGE_ENTRIES];
} page_directory_t;

typedef struct {
  pte_t entries[PAGE_ENTRIES];
} page_table_t;

void vmm_init(uintptr_t kernel_stack_pointer);
uint32_t vmm_get_physical_addr(uint32_t virtual_addr);
uint32_t vmm_get_page_offset(uint32_t virtual_addr);
void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr,
                  uint32_t flags);
void vmm_unmap_page(uint32_t virtual_addr);
uintptr_t vmm_alloc_page(uintptr_t virtual_addr, uint32_t flags);
uint32_t vmm_alloc_pages(uint32_t from, uint32_t to, uint32_t count,
                         uint32_t flags);
uint32_t vmm_alloc_user_pages(uint32_t);
uint32_t vmm_alloc_kernel_pages(uint32_t);
uint32_t vmm_alloc_kernel_heap_pages(uint32_t);
uint32_t vmm_get_needed_pages(uint64_t bytes);
bool vmm_test_if_mapped(uint32_t virtual_addr);

#endif