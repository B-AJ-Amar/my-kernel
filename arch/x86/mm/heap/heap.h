
#ifndef HEAP_H
#define HEAP_H

#include <mm/mm.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct heap_block_t {
  uint32_t size;
  bool free;
  struct heap_block_t *prev;
  struct heap_block_t *next;
} heap_block_t;

void k_heap_init(void);
uint32_t kmalloc(uint32_t size);
uint32_t kfree(uint32_t ptr);

uint32_t kcalloc(uint32_t num, uint32_t size);
uint32_t krealloc(uint32_t ptr, uint32_t new_size);

#endif