
#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define FRAME_SIZE 4096
#define MAX_PHYS_MEMORY 0x100000000ULL
#define MAX_FRAMES (MAX_PHYS_MEMORY / FRAME_SIZE)
#define BITMAP_SIZE (MAX_FRAMES / 8)

#define E820_MAX_ENTRIES 128
#define E820_TYPE_USABLE 1
#define E820_TYPE_RESERVED 2
#define E820_TYPE_ACPI_RECLAIM 3
#define E820_TYPE_ACPI_NVS 4
#define E820_TYPE_BAD_MEMORY 5

typedef struct {
  uint64_t base;
  uint64_t length;
  uint32_t type;
  uint32_t acpi;
} __attribute__((packed)) e820_entry_t;

typedef struct {
  uint32_t count;
  e820_entry_t *entries;
} e820_map_t;

void pmm_init(uint64_t kernel_addr, uint64_t kernel_size,
              uint32_t e820_entries_count, uint32_t e820_entries_addr);

uint32_t pmm_alloc_frame(void);
uint32_t pmm_alloc_empty_frame(void);
void pmm_free_frame(uint32_t address);

#endif