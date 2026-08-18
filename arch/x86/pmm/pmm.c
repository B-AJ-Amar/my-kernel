#include <pmm/pmm.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FRAME_SIZE 4096
#define MAX_PHYS_MEMORY 0x100000000ULL
#define MAX_FRAMES (MAX_PHYS_MEMORY / FRAME_SIZE)
#define BITMAP_SIZE (MAX_FRAMES / 8)

static void mem_set_range(uint64_t base, uint64_t length, bool is_reserved);
static void bitmap_set_range(uint32_t start_frame, uint32_t num_frames,
                             bool is_reserved);

static e820_map_t e820_entries;
static uint8_t frame_bitmap[BITMAP_SIZE];

void pmm_init(uint64_t kernel_addr, uint64_t kernel_size,
              uint64_t e820_entries_count, uint32_t e820_entries_addr) {
  e820_entries.entries = (e820_entry_t *)e820_entries_addr;
  e820_entries.count = e820_entries_count;
  memset(frame_bitmap, 0xFF, sizeof(frame_bitmap));

  for (uint32_t i = 0; i < e820_entries.count; i++) {
    e820_entry_t *entry = &e820_entries.entries[i];
    if (entry->type == E820_TYPE_USABLE) {
      mem_set_range(entry->base, entry->length, false);
    }
  }

  mem_set_range(kernel_addr, kernel_size, true);
  mem_set_range(0, 0x100000, true); // ? first 1mb (real mode stuff)
}

static void bitmap_reserve(uint32_t frame) {
  frame_bitmap[frame / 8] |= (uint8_t)(1 << (frame % 8));
}

static void bitmap_clear(uint32_t frame) {
  frame_bitmap[frame / 8] &= (uint8_t)~(1 << (frame % 8));
}

static bool bitmap_test(uint32_t frame) {
  return (frame_bitmap[frame / 8] & (uint8_t)(1 << (frame % 8))) != 0;
}

static void bitmap_set_range(uint32_t start_frame, uint32_t num_frames,
                             bool is_reserved) {
  if (is_reserved) {
    for (uint32_t i = 0; i < num_frames; i++) {
      bitmap_reserve(start_frame + i);
    }
  } else {
    for (uint32_t i = 0; i < num_frames; i++) {
      bitmap_clear(start_frame + i);
    }
  }
}

static void mem_set_range(uint64_t base, uint64_t length, bool is_reserved) {
  if (length == 0 || base >= MAX_PHYS_MEMORY || base + length > MAX_PHYS_MEMORY)
    return;
  uint32_t start_frame = base / FRAME_SIZE;
  uint32_t num_frames = (length + FRAME_SIZE - 1) / FRAME_SIZE;
  if (is_reserved) {
    bitmap_set_range(start_frame, num_frames, true);
  } else {
    bitmap_set_range(start_frame, num_frames, false);
  }
}

uint32_t pmm_alloc_frame(void) {
  for (uint32_t frame = 0; frame < MAX_FRAMES; frame++) {
    if (!bitmap_test(frame)) {
      bitmap_reserve(frame);
      return frame * FRAME_SIZE;
    }
  }
  return 0;
}

uint32_t pmm_alloc_empty_frame(void) {
  uint32_t addr = pmm_alloc_frame();
  if (addr != 0) {
    memset((void *)(uint32_t)addr, 0, FRAME_SIZE);
    return addr;
  }
  return 0;
}

void pmm_free_frame(uint32_t address) {
  if (address % FRAME_SIZE != 0 || address >= MAX_PHYS_MEMORY) {
    return; // Invalid address
  }
  uint32_t frame = address / FRAME_SIZE;
  bitmap_clear(frame);
}