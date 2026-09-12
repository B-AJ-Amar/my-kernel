
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
void *kmalloc(uint32_t size);
void kfree(void *ptr);

void *kcalloc(uint32_t num, uint32_t size);
void *krealloc(void *ptr, uint32_t new_size);

#endif