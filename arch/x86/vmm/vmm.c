

#include <pmm/pmm.h>
#include <stdio.h>
#include <string.h>
#include <vmm/vmm.h>

static void enable_paging(uint32_t page_dir);
static page_directory_t *kernel_page_dir;

void vmm_init(void) {
  uint32_t page_dir = pmm_alloc_frame();
  uint32_t table_frame = pmm_alloc_frame();

  printf("[\033[2,0]x\033[15,0]] page dir allocated at: 0x%x\n", page_dir);
  printf("[\033[2,0]x\033[15,0]] page table allocated at: 0x%x\n", table_frame);
  if (table_frame == 0 || page_dir == 0) {
    panic("Failed to allocate memory for page directory or page table");
  }

  page_table_t *table = (page_table_t *)table_frame;
  kernel_page_dir = (page_directory_t *)page_dir;

  memset(kernel_page_dir, 0, PAGE_SIZE);
  memset(table, 0, PAGE_SIZE);

  //? map the first 4mb of memory: (1mb (bios and bootloader) + 3mb (kernel))
  for (uint32_t i = 0; i < PAGE_ENTRIES; i++) {
    table->entries[i] =
        ((i * PAGE_SIZE) & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;
  }

  kernel_page_dir->entries[0] =
      (table_frame & 0xFFFFF000) | PAGE_F_PRESENT | PAGE_F_WRITABLE;

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