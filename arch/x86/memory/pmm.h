
#ifndef PMM_H
#define PMM_H

#define E820_MAX_ENTRIES 128

#include <stdint.h>

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed)) e820_entry_t;

typedef struct {
    uint32_t count;
    e820_entry_t entries[E820_MAX_ENTRIES];
} memory_map_t;


void pmm_init();

#endif