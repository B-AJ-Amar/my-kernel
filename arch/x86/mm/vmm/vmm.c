#include <mm/pmm/pmm.h>
#include <mm/vmm/vmm.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * TODO:
 * - page fault handling
 * - better virtual address allocator
 */

static void enable_paging(uint32_t page_dir);

static void _decode_virtual_addr(uint32_t virtual_addr, uint32_t *dir_idx,
                                 uint32_t *table_idx, uint32_t *offset);

static page_directory_t *kernel_page_dir;

/*

 * PDE[1023] points back to the page directory.
 *
 * Therefore:
 *
 *   0xFFFFF000 -> page directory
 *
 *   0xFFC00000 -> page table 0
 *   0xFFC01000 -> page table 1
 *   ...
 *   0xFFFFE000 -> page table 1022 (for now i will use the stack for the threads
 only and keep the main thread stack in the identity maped area)

 */

static inline page_directory_t *vmm_get_page_directory(void) {
  return (page_directory_t *)RECURSIVE_PD_ADDR;
}

static inline page_table_t *vmm_get_page_table(uint32_t dir_index) {
  return (page_table_t *)(RECURSIVE_PT_BASE + (dir_index * PAGE_SIZE));
}

void vmm_init(uintptr_t sp_top, uintptr_t sp_bottom) {
  // ?for now i will not allocate the stack frames and i will keep them to the
  // identiry handler

  // sp_top--;

  // uintptr_t stack_frames[
  //     (sp_top - sp_bottom) / PAGE_SIZE + 1
  // ];

  // uint32_t stack_frame_count = 0;

  // for (uintptr_t addr = sp_top;
  //      addr >= sp_bottom;
  //      addr -= PAGE_SIZE) {

  //     stack_frames[stack_frame_count++] =
  //         pmm_alloc_specific_frame(addr);
  // }

  // ? alloc page dir and page table for the kernel
  uint32_t page_dir = pmm_alloc_empty_frame();
  uint32_t table_frame = pmm_alloc_empty_frame();

  printf("[\033[2,0]x\033[15,0]] page dir allocated at: 0x%x\n", page_dir);

  printf("[\033[2,0]x\033[15,0]] page table allocated at: 0x%x\n", table_frame);

  if (table_frame == 0 || page_dir == 0) {
    panic("Failed to allocate memory for page directory or page table");
  }

  // ? identity map the first 4 MB of physical memory
  page_table_t *table = (page_table_t *)table_frame;
  kernel_page_dir = (page_directory_t *)page_dir;

  for (uint32_t i = 0; i < PAGE_ENTRIES; i++) {
    table->entries[i] = (i * PAGE_SIZE) | PAGE_F_PRESENT | PAGE_F_WRITABLE;
  }

  kernel_page_dir->entries[0] =
      (table_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

  // ? for now i will keep the kernel stack in the identity mapped area and i
  // will not allocate a page table for it page_table_t *kernel_stack_table =
  //     (page_table_t *)pmm_alloc_empty_frame();

  // if (kernel_stack_table == NULL) {
  //     panic("Failed to allocate memory for kernel stack page table");
  // }

  // for (uint32_t i = 0;
  //      i < stack_frame_count;
  //      i++) {

  //     kernel_stack_table->entries[1023 - i] =
  //         (stack_frames[i] & 0xFFFFF000) |
  //         PAGE_F_PRESENT |
  //         PAGE_F_WRITABLE;
  // }

  // kernel_page_dir->entries[KERNEL_STACK_PDE_INDEX] =
  //     ((uintptr_t)kernel_stack_table & 0xFFFFF000) |
  //     PAGE_F_PRESENT |
  //     PAGE_F_WRITABLE;

  // ? recursive paging

  kernel_page_dir->entries[RECURSIVE_PDE_INDEX] =
      ((uintptr_t)page_dir & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

  asm volatile("cli");

  enable_paging(page_dir);

  kernel_page_dir = vmm_get_page_directory();

  asm volatile("sti");
}

static void enable_paging(uint32_t page_dir) {
  asm volatile(".intel_syntax noprefix\n\t"
               "mov cr3, %0\n\t"

               "mov eax, cr0\n\t"

               /*
                * PE  = bit 0
                * PG  = bit 31
                */
               "or eax, 0x80000001\n\t"

               "mov cr0, eax\n\t"

               ".att_syntax prefix\n\t"
               :
               : "r"(page_dir)
               : "eax", "memory");
}

/*
   31             22 21             12 11             0
   +---------------+-----------------+----------------+
   |   PDE index   |   PTE index     |    offset      |
   |    10 bits    |    10 bits      |    12 bits     |
   +---------------+-----------------+----------------+
*/

static void _decode_virtual_addr(uint32_t virtual_addr, uint32_t *dir_idx,
                                 uint32_t *table_idx, uint32_t *offset) {
  *dir_idx = (virtual_addr >> 22) & 0x3FF;

  *table_idx = (virtual_addr >> 12) & 0x3FF;

  *offset = virtual_addr & 0xFFF;
}

uint32_t vmm_get_physical_addr(uint32_t virtual_addr) {
  uint32_t dir_index;
  uint32_t table_index;

  _decode_virtual_addr(virtual_addr, &dir_index, &table_index, NULL);

  page_directory_t *pd = vmm_get_page_directory();

  pde_t page_dir_entry = pd->entries[dir_index];

  if (!(page_dir_entry & PAGE_F_PRESENT)) {
    return 0;
  }

  /*
   * we dont need to convert the physical page table address
   * into a pointer
   *
   * recursive paging gives us the correct virtual address.
   */

  page_table_t *page_table = vmm_get_page_table(dir_index);

  pte_t page_table_entry = page_table->entries[table_index];

  if (!(page_table_entry & PAGE_F_PRESENT)) {
    return 0;
  }

  return (page_table_entry & 0xFFFFF000) | (virtual_addr & 0xFFF);
}

void vmm_map_page(uint32_t virtual_addr, uint32_t physical_addr,
                  uint32_t flags) {
  uint32_t dir_index = (virtual_addr >> 22) & 0x3FF;

  uint32_t table_index = (virtual_addr >> 12) & 0x3FF;

  page_directory_t *pd = vmm_get_page_directory();

  pde_t page_dir_entry = pd->entries[dir_index];

  if (dir_index == RECURSIVE_PDE_INDEX) {
    panic("Attempt to map into recursive paging region");
  }

  if (!(page_dir_entry & PAGE_F_PRESENT)) {

    uint32_t new_table_frame = pmm_alloc_frame();

    if (new_table_frame == 0) {
      panic("Failed to allocate memory for new page table");
    }

    /*
     * At this point the new page table is not yet mapped
     * into a convenient virtual address.
     *
     * Since your current physical memory below 4 MB is
     * identity mapped, this is only safe if the allocator
     * returns a frame inside the identity-mapped region.
     *
     * Better: zero it through the recursive mapping after
     * installing the PDE.
     */

    pd->entries[dir_index] =
        (new_table_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

    /*
     * Now the recursive mapping gives us access to the
     * newly created page table.
     */

    page_table_t *new_table = vmm_get_page_table(dir_index);

    memset(new_table, 0, PAGE_SIZE);
  }

  page_table_t *page_table = vmm_get_page_table(dir_index);

  page_table->entries[table_index] =
      (physical_addr & 0xFFFFF000) | (flags & 0xFFF) | PAGE_F_PRESENT;

  /*
   * Flush the translation for this virtual page.
   */

  asm volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");
}

void vmm_unmap_page(uint32_t virtual_addr) {
  uint32_t dir_index;
  uint32_t table_index;

  _decode_virtual_addr(virtual_addr, &dir_index, &table_index, NULL);

  if (dir_index == RECURSIVE_PDE_INDEX) {
    panic("Attempt to unmap recursive paging region");
  }

  page_directory_t *pd = vmm_get_page_directory();

  pde_t page_dir_entry = pd->entries[dir_index];

  if (!(page_dir_entry & PAGE_F_PRESENT)) {
    return;
  }

  page_table_t *page_table = vmm_get_page_table(dir_index);

  pte_t page_table_entry = page_table->entries[table_index];

  if (!(page_table_entry & PAGE_F_PRESENT)) {
    return;
  }

  // save physical page before clearing the PTE.

  uint32_t physical_addr = page_table_entry & 0xFFFFF000;

  page_table->entries[table_index] = 0;

  asm volatile("invlpg (%0)" : : "r"(virtual_addr) : "memory");


  pmm_free_frame(physical_addr);

  /*
   * IMPORTANT:
   *
   * we DO NOT do:
   *
   *     pd->entries[dir_index] &= ~PAGE_F_PRESENT;
   *
   * because other PTEs may still exist in this page table
   */
}

uint32_t vmm_alloc_user_page(void) {
  uint32_t physical_addr = pmm_alloc_frame();

  if (physical_addr == 0) {
    return 0;
  }

  for (uint32_t virtual_addr = USER_SPACE_BASE; virtual_addr < USER_SPACE_END;
       virtual_addr += PAGE_SIZE) {

    if (vmm_get_physical_addr(virtual_addr) == 0) {

      vmm_map_page(virtual_addr, physical_addr, PAGE_F_WRITABLE | PAGE_F_USER);

      return virtual_addr;
    }
  }

  pmm_free_frame(physical_addr);

  return 0;
}

uintptr_t vmm_alloc_page(uintptr_t virtual_addr, uint32_t flags) {
  uint32_t physical_addr = pmm_alloc_frame();

  if (physical_addr == 0) {
    return 0;
  }

  virtual_addr = (virtual_addr / PAGE_SIZE) * PAGE_SIZE;

  /*
   * Do not allow allocation inside recursive paging.
   */

  if ((virtual_addr >> 22) == RECURSIVE_PDE_INDEX) {
    pmm_free_frame(physical_addr);
    return 0;
  }

  vmm_map_page(virtual_addr, physical_addr, flags);

  return virtual_addr;
}

uint32_t vmm_alloc_pages(uint32_t from, uint32_t to, uint32_t count,
                         uint32_t flags) {
  if (!count) {
    return 0;
  }

  uint32_t from_addr = (from / PAGE_SIZE) * PAGE_SIZE;

  uint32_t to_addr = (to / PAGE_SIZE) * PAGE_SIZE;

  uint32_t vpages_count = 0;
  uint32_t virtual_addr;

  // find a contiguous virtual range.

  for (virtual_addr = from_addr; virtual_addr <= to_addr;
       virtual_addr += PAGE_SIZE) {

    // never allocate in recursive paging region.

    if ((virtual_addr >> 22) == RECURSIVE_PDE_INDEX) {

      break;
    }

    if (vmm_get_physical_addr(virtual_addr) == 0) {

      vpages_count++;

      if (vpages_count == count) {
        break;
      }

    } else {
      vpages_count = 0;
    }
  }

  if (vpages_count == 0) {
    return 0;
  }

  virtual_addr -= vpages_count * PAGE_SIZE;

  uint32_t frames[vpages_count];


  for (uint32_t i = 0; i < vpages_count; i++) {

    uint32_t physical_addr = pmm_alloc_frame();

    if (physical_addr == 0) {

      // ? roll back already allocated frames.
       

      for (uint32_t j = 0; j < i; j++) {

        pmm_free_frame(frames[j]);

        vmm_unmap_page(virtual_addr + j * PAGE_SIZE);
      }

      return 0;
    }

    frames[i] = physical_addr;

    vmm_map_page(virtual_addr + i * PAGE_SIZE, physical_addr, flags);
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
  if (!bytes) {
    return 0;
  }

  return ((bytes - 1) / PAGE_SIZE) + 1;
}

uint32_t vmm_get_page_offset(uint32_t virtual_addr) {
  return virtual_addr & 0xFFF;
}

bool vmm_test_if_mapped(uint32_t virtual_addr) {
  return vmm_get_physical_addr(virtual_addr) != 0;
}