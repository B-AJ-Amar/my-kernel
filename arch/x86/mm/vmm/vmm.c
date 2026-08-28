

#include <mm/pmm/pmm.h>
#include <mm/vmm/vmm.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
// TODO: add page fault handling
uint32_t get_pages_count(uint32_t size);
static void enable_paging(uint32_t page_dir);
static void _decode_virtual_addr(uint32_t virtual_addr, uint32_t *dir_idx,
                                 uint32_t *table_idx, uint32_t *offset);
static page_directory_t *kernel_page_dir;
/*
PT: each page tale holds 1024 entries each entry points to 4kb page, so each
page table can map 4mb of memory PDT: 1024 entries so each page directory can
map 4gb of memory

*/
void vmm_init(void) {
  uint32_t page_dir = pmm_alloc_empty_frame();
  uint32_t table_frame = pmm_alloc_empty_frame();

  printf("[\033[2,0]x\033[15,0]] page dir allocated at: 0x%x\n", page_dir);
  printf("[\033[2,0]x\033[15,0]] page table allocated at: 0x%x\n", table_frame);
  if (table_frame == 0 || page_dir == 0) {
    panic("Failed to allocate memory for page directory or page table");
  }

  page_table_t *table = (page_table_t *)table_frame;
  kernel_page_dir = (page_directory_t *)page_dir;

  //? map the first 4mb of memory: (1mb (bios and bootloader) + 3mb (kernel))
  for (uint32_t i = 0; i < PAGE_ENTRIES; i++) {
    table->entries[i] =
        ((i * PAGE_SIZE) & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;
  }

  kernel_page_dir->entries[0] =
      (table_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

  // ? mape the stack addr from KERNEL_RM_SP to 0xFFFFFFFF
  page_table_t *kernel_stack_table = (page_table_t *)pmm_alloc_empty_frame();
  if (kernel_stack_table == NULL) {
    panic("Failed to allocate memory for kernel stack page table");
  }

  uintptr_t kernel_stack_frame = pmm_alloc_specific_frame(KERNEL_RM_SP);
  if (kernel_stack_frame == 0) {
    panic("Failed to allocate memory for kernel stack frame");
  }

  kernel_stack_table->entries[1023] =
      (kernel_stack_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

  kernel_page_dir->entries[1023] =
      ((uintptr_t)kernel_stack_table & 0xFFFFF000) | PAGE_F_PRESENT |
      PAGE_F_WRITABLE;

  asm volatile("cli");
  enable_paging(page_dir);
  asm volatile("sti");
}

static void enable_paging(uint32_t page_dir) {
  asm volatile(".intel_syntax noprefix\n\t"
               "mov cr3, %0\n\t"
               "mov eax, cr0\n\t"
               "or eax, 0x80000001\n\t"
               "mov cr0, eax\n\t"
               ".att_syntax prefix\n\t"
               :
               : "r"(page_dir)
               : "eax", "memory");
}

// ┌──────────────┬──────────────┬────────────┐
// │ PDE index    │ PTE index    │ Offset     │
// │ 10 bits      │ 10 bits      │ 12 bits    │
// └──────────────┴──────────────┴────────────┘

uint32_t vmm_get_physical_addr(uint32_t virtual_addr) {
  uint32_t dir_index, table_index;
  _decode_virtual_addr(virtual_addr, &dir_index, &table_index, NULL);

  pde_t page_dir_entry = kernel_page_dir->entries[dir_index];
  if (!(page_dir_entry & PAGE_F_PRESENT)) {
    return 0; // Page directory entry not present
  }

  page_table_t *page_table = (page_table_t *)(page_dir_entry & 0xFFFFF000);
  pte_t page_table_entry = page_table->entries[table_index];
  if (!(page_table_entry & PAGE_F_PRESENT)) {
    return 0; // Page table entry not present
  }

  return (page_table_entry & 0xFFFFF000) | (virtual_addr & 0xFFF);
}

void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr,
                  uint32_t flags) {
  uint32_t dir_index = (virtual_addr >> 22) & 0x3FF;
  uint32_t table_index = (virtual_addr >> 12) & 0x3FF;

  pde_t page_dir_entry = kernel_page_dir->entries[dir_index];
  page_table_t *page_table;

  if (!(page_dir_entry & PAGE_F_PRESENT)) {
    uint32_t new_table_frame = pmm_alloc_frame();
    if (new_table_frame == 0) {
      panic("Failed to allocate memory for new page table");
    }
    memset((void *)new_table_frame, 0, PAGE_SIZE);
    kernel_page_dir->entries[dir_index] =
        (new_table_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;
    page_table = (page_table_t *)new_table_frame;
  } else {
    page_table = (page_table_t *)(page_dir_entry & 0xFFFFF000);
  }

  page_table->entries[table_index] =
      (physical_addr & 0xFFFFF000) | (flags & 0xFFF) | PAGE_F_PRESENT;
}

void vmm_unmap_page(uint32_t virtual_addr) {
  uint32_t dir_index, table_index;
  _decode_virtual_addr(virtual_addr, &dir_index, &table_index, NULL);

  pde_t page_dir_entry = kernel_page_dir->entries[dir_index];
  if (!(page_dir_entry & PAGE_F_PRESENT)) {
    return; // page dir entry not present
  }

  page_table_t *page_table = (page_table_t *)(page_dir_entry & 0xFFFFF000);
  pte_t page_table_entry = page_table->entries[table_index];
  if (!(page_table_entry & PAGE_F_PRESENT)) {
    return; // page table entry not present
  }

  page_table->entries[table_index] = 0;
  kernel_page_dir->entries[dir_index] &= ~PAGE_F_PRESENT;
  pmm_free_frame(page_table_entry & 0xFFFFF000);
}

uint32_t vmm_alloc_page(void) {
  uint32_t physical_addr = pmm_alloc_frame();
  if (physical_addr == 0) {
    return 0;
  }
}

static void _decode_virtual_addr(uint32_t virtual_addr, uint32_t *dir_idx,
                                 uint32_t *table_idx, uint32_t *offset) {
  *dir_idx = (virtual_addr >> 22) & 0x3FF;
  *table_idx = (virtual_addr >> 12) & 0x3FF;
  *offset = virtual_addr & 0xFFF;
}

uint32_t vmm_alloc_user_page(void) {
  uint32_t physical_addr = pmm_alloc_frame();
  if (physical_addr == 0) {
    return 0;
  }

  for (uint32_t virtual_addr = USER_SPACE_BASE; virtual_addr < USER_SPACE_END;
       virtual_addr += PAGE_SIZE) {
    if (vmm_get_physical_addr(virtual_addr) == 0) {
      vmm_map_page(virtual_addr, physical_addr,
                   PAGE_F_PRESENT | PAGE_F_WRITABLE | PAGE_F_USER);
      return virtual_addr;
    }
  }

  pmm_free_frame(physical_addr);
  return 0;
}

uint32_t vmm_alloc_pages(uint32_t from, uint32_t to, uint32_t count,
                         uint32_t flags) {
  if (!count)
    return 0;
  uint32_t from_addr = (from / PAGE_SIZE) * PAGE_SIZE;
  uint32_t to_addr = (to / PAGE_SIZE) * PAGE_SIZE;
  uint32_t vpages_count = 0, virtual_addr;
  for (virtual_addr = from_addr; virtual_addr <= to_addr;
       virtual_addr += PAGE_SIZE) {
    if (vmm_get_physical_addr(virtual_addr) == 0) {
      vpages_count++;
      if (vpages_count == count)
        break;
    } else {
      vpages_count = 0;
    }
  }
  if (vpages_count == 0)
    return 0;

  virtual_addr -= vpages_count * PAGE_SIZE;
  uint32_t frames[vpages_count];
  for (int i = 0; i < vpages_count; i++) {
    uint32_t physical_addr = pmm_alloc_frame();
    if (physical_addr == 0) {
      for (int j = 0; j < i; j++) {
        pmm_free_frame(frames[j]);
      }
      return 0;
    }
    frames[i] = physical_addr;
    vmm_map_page(virtual_addr + i * PAGE_SIZE, physical_addr,
                 flags | PAGE_F_PRESENT);
  }

  return virtual_addr;
}

uint32_t vmm_alloc_kernel_pages(uint32_t count) {
  return vmm_alloc_pages(KERNEL_SPACE_BASE, KERNEL_SPACE_END, count,
                         PAGE_F_WRITABLE);
}

uint32_t vmm_alloc_kernel_heap_pages(uint32_t count) {
  return vmm_alloc_pages(KERNEL_HEAP_START, KERNEL_HEAP_END, count,
                         PAGE_F_WRITABLE);
}

uint32_t vmm_alloc_user_pages(uint32_t count) {
  return vmm_alloc_pages(USER_SPACE_BASE, USER_SPACE_END, count,
                         PAGE_F_WRITABLE | PAGE_F_USER);
}

uint32_t vmm_get_needed_pages(uint64_t bytes) {
  if (!bytes)
    return 0;
  return ((bytes - 1) / PAGE_SIZE) + 1;
}

uint32_t vmm_get_page_offset(uint32_t virtual_addr) {
  return virtual_addr & 0xFFF;
}

bool vmm_test_if_mapped(uint32_t virtual_addr) {
  return vmm_get_physical_addr(virtual_addr) != 0;
}