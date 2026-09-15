#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t new_size);
void free(void *ptr);

#endif
