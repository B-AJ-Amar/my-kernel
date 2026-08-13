
#include <stdint.h>

#define BOOT_INFO_ADDR 0x7000

typedef struct {
  uint32_t kernel_addr;
  uint32_t kernel_size;
  uint32_t e820_entries_count;
  uint32_t e820_entries_addr;
  uint32_t e820_max_entries;
  uint32_t e820_entry_size;
} boot_info_t;
