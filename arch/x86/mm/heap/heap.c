
#include <mm/heap/heap.h>
#include <mm/vmm/vmm.h>
#include <stddef.h>

static heap_block_t *kernel_heap_head = NULL;

void k_heap_init(void) {
  uint32_t heap_start_addr = vmm_alloc_kernel_pages(1);
  kernel_heap_head = (heap_block_t *)heap_start_addr;
  kernel_heap_head[0].size = PAGE_SIZE - sizeof(heap_block_t);
  kernel_heap_head[0].free = true;
  kernel_heap_head[0].prev = NULL;
  kernel_heap_head[0].next = NULL;
}

static uint32_t create_block(uint32_t size, heap_block_t *heap_head,
                             uint32_t (*alloc_pages_to_heap)(uint32_t));
static uint32_t free_block(uint32_t ptr, heap_block_t *heap_head);
// first fit
uint32_t kmalloc(uint32_t size) {

  if (size == 0 || size > KERNEL_HEAP_SIZE) {
    return 0;
  }

  return create_block(size, kernel_heap_head, vmm_alloc_kernel_heap_pages);
}

uint32_t kfree(uint32_t ptr) { return free_block(ptr, kernel_heap_head); }

uint32_t kcalloc(uint32_t num, uint32_t size) {
  uint32_t total_size = num * size;
  uint32_t ptr = kmalloc(total_size);
  if (ptr == 0) {
    return 0;
  }
  memset((void *)ptr, 0, total_size);
  return ptr;
}

uint32_t krealloc(uint32_t ptr, uint32_t new_size) {
  if (ptr == 0) {
    return kmalloc(new_size);
  }

  if (new_size == 0) {
    kfree(ptr);
    return 0;
  }

  heap_block_t *block = (heap_block_t *)(ptr - sizeof(heap_block_t));
  uint32_t old_size = block->size;

  if (new_size <= old_size) {
    return ptr;
  }

  uint32_t new_ptr = kmalloc(new_size);
  if (new_ptr == 0) {
    return 0;
  }

  memcpy((void *)new_ptr, (void *)ptr, old_size);
  kfree(ptr);

  return new_ptr;
}

uint32_t create_block(uint32_t size, heap_block_t *heap_head,
                      uint32_t (*alloc_pages_to_heap)(uint32_t)) {

  heap_block_t *current = heap_head;
  while (current->next != NULL) {
    if (current->free && current->size >= size + sizeof(heap_block_t)) {
      uint32_t new_block_addr = (uint32_t)(current + 1) + size;
      heap_block_t *new_block = (heap_block_t *)new_block_addr;
      new_block->size = current->size - size - sizeof(heap_block_t);
      new_block->free = true;
      new_block->prev = current;
      new_block->next = current->next;

      current->size = size;
      current->next = new_block;

      if (new_block->next != NULL) {
        new_block->next->prev = new_block;
      }

      return (uint32_t)(current + 1);
    }
  }
  if (current->free && current->size >= size + sizeof(heap_block_t)) {
    uint32_t new_block_addr = (uint32_t)(current + 1) + size;
    heap_block_t *new_block = (heap_block_t *)new_block_addr;
    new_block->size = current->size - size - sizeof(heap_block_t);
    new_block->free = true;
    new_block->prev = current;
    new_block->next = NULL;

    current->size = size;
    current->next = new_block;

    return (uint32_t)(current + 1);
  } else {
    uint32_t pages_needed = vmm_get_needed_pages(size + sizeof(heap_block_t));
    uint32_t new_block_addr = alloc_pages_to_heap(pages_needed);
    if (new_block_addr == 0) {
      return 0;
    }

    heap_block_t *new_block = (heap_block_t *)new_block_addr;
    new_block->size = pages_needed * PAGE_SIZE - sizeof(heap_block_t);
    new_block->free = true;
    new_block->prev = current;
    new_block->next = NULL;

    current->next = new_block;

    return create_block(size, new_block, alloc_pages_to_heap);
  }

  return 0;
}

uint32_t free_block(uint32_t ptr, heap_block_t *heap_head) {
  if (ptr == 0) {
    return 0;
  }

  heap_block_t *block_to_free = (heap_block_t *)(ptr - sizeof(heap_block_t));
  block_to_free->free = true;

  if (block_to_free->next != NULL && block_to_free->next->free) {
    block_to_free->size += sizeof(heap_block_t) + block_to_free->next->size;
    block_to_free->next = block_to_free->next->next;
    if (block_to_free->next != NULL) {
      block_to_free->next->prev = block_to_free;
    }
  }

  if (block_to_free->prev != NULL && block_to_free->prev->free) {
    block_to_free->prev->size += sizeof(heap_block_t) + block_to_free->size;
    block_to_free->prev->next = block_to_free->next;
    if (block_to_free->next != NULL) {
      block_to_free->next->prev = block_to_free->prev;
    }
  }

  return 1;
}