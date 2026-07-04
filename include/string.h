#ifndef STRING_H
#define STRING_h

#include <stddef.h>

void *memcpy(void *dest_str, const void *src_str, size_t len);
void *memmove(void *dest, const void *src, size_t len);
void *memset(void *dest, int val, size_t len);
int memcmp(const void *str1, const void *str2, size_t count);
size_t strlen(char *str);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strcat(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

#endif