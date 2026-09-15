#include <mm/heap/heap.h>
#include <stdint.h>
#include <stddef.h>

// TODO: use userspace insteead of kernel space
void *malloc(size_t size) { return kmalloc(size); }
void *calloc(size_t num, size_t size) { return kcalloc(num, size); }
void *realloc(void *ptr, size_t new_size) { return krealloc(ptr, new_size); }
void free(void *ptr) { kfree(ptr); }