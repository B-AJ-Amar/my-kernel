#include <memory/pmm.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 4096
#define MAX_PHYS_MEMORY 0x100000000ULL
#define MAX_PAGES (MAX_PHYS_MEMORY / PAGE_SIZE)
#define BITMAP_SIZE (MAX_PAGES / 8)

static void mem_set_range(uint64_t base, uint64_t length, bool is_reserved);
static void bitmap_set_range(uint32_t start_page, uint32_t num_pages,
                             bool is_reserved);

static e820_map_t e820_entries;
static uint8_t page_bitmap[BITMAP_SIZE];

void pmm_init(uint64_t kernel_addr, uint64_t kernel_size,
              uint64_t e820_entries_count, uint64_t e820_entries_addr) {
  e820_entries.entries = (e820_entry_t *)e820_entries_addr;
  e820_entries.count = e820_entries_count;
  memset(page_bitmap, 0xFF, sizeof(page_bitmap));

  for (uint32_t i = 0; i < e820_entries.count; i++) {
    e820_entry_t *entry = &e820_entries.entries[i];
    if (entry->type == E820_TYPE_USABLE) {
      mem_set_range(entry->base, entry->length, false);
    }
  }

  mem_set_range(kernel_addr, kernel_size, true);
  mem_set_range(0, 0x100000, true); // ? first 1mb (real mode stuff)
}

static void bitmap_reserve(uint32_t page) {
  page_bitmap[page / 8] |= (uint8_t)(1 << (page % 8));
}

static void bitmap_clear(uint32_t page) {
  page_bitmap[page / 8] &= (uint8_t)~(1 << (page % 8));
}

static bool bitmap_test(uint32_t page) {
  return (page_bitmap[page / 8] & (uint8_t)(1 << (page % 8))) != 0;
}

static void bitmap_set_range(uint32_t start_page, uint32_t num_pages,
                             bool is_reserved) {
  if (is_reserved) {
    for (uint32_t i = 0; i < num_pages; i++) {
      bitmap_reserve(start_page + i);
    }
  } else {
    for (uint32_t i = 0; i < num_pages; i++) {
      bitmap_clear(start_page + i);
    }
  }
}

static void mem_set_range(uint64_t base, uint64_t length, bool is_reserved) {
  if (length == 0 || base >= MAX_PHYS_MEMORY || base + length > MAX_PHYS_MEMORY)
    return;
  uint32_t start_page = base / PAGE_SIZE;
  uint32_t num_pages = (length + PAGE_SIZE - 1) / PAGE_SIZE;
  if (is_reserved) {
    bitmap_set_range(start_page, num_pages, true);
  } else {
    bitmap_set_range(start_page, num_pages, false);
  }
}

uint32_t pmm_alloc_page(void) {
  for (uint32_t page = 0; page < MAX_PAGES; page++) {
    if (!bitmap_test(page)) {
      bitmap_reserve(page);
      return page * PAGE_SIZE;
    }
  }
  return 0;
}

void pmm_free_page(uint32_t address) {
  if (address % PAGE_SIZE != 0 || address >= MAX_PHYS_MEMORY) {
    return; // Invalid address
  }
  uint32_t page = address / PAGE_SIZE;
  bitmap_clear(page);
}