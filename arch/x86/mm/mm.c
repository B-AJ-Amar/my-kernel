#include <mm/heap/heap.h>
#include <mm/mm.h>
#include <mm/pmm/boot_info.h>
#include <mm/pmm/pmm.h>
#include <mm/vmm/vmm.h>

void init_mm(boot_info_t *boot) {
  pmm_init(boot->kernel_addr, boot->kernel_size, boot->e820_entries_count,
           boot->e820_entries_addr);
  vmm_init();
  k_heap_init();
}