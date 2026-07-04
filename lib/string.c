

#include <string.h>

void *memcpy(void *dest, const void *src, size_t len) {
  char *d = dest;
  const char *s = src;
  while (len--)
    *d++ = *s++;
  return dest;
}
void *memmove(void *dest, const void *src, size_t len) {
  char *d = dest;
  const char *s = src;
  if (d < s)
    while (len--)
      *d++ = *s++;
  else {
    char *lasts = s + (len - 1);
    char *lastd = d + (len - 1);
    while (len--)
      *lastd-- = *lasts--;
  }
  return dest;
}

void *memset(void *dest, int val, size_t len) {
  unsigned char *ptr = dest;
  while (len-- > 0)
    *ptr++ = val;
  return dest;
}

int memcmp(const void *str1, const void *str2, size_t count) {
  const unsigned char *s1 = str1;
  const unsigned char *s2 = str2;

  while (count-- > 0) {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;
}

size_t strlen(char *str) {
  size_t len = 0;
  while (*str != '\0') {
    str++;
    len++;
  }
  return len;
}

char *strcpy(char *dest, const char *src) {
  char *ptr = dest;
  while (*src != '\0') {
    *ptr++ = *src++;
  }
  *ptr = '\0';
  return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
  return memcpy(dest, src, n);
}

char *strcat(char *dest, const char *src) {
  char *ptr = dest + strlen(dest);
  while (*src != '\0') {
    *ptr++ = *src++;
  }
  *ptr = '\0';
  return dest;
}

int strcmp(const char *s1, const char *s2) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while (*p1 != '\0' && *p1 == *p2) {
    p1++;
    p2++;
  }
  return *p1 - *p2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  while (n-- > 0 && *p1 != '\0' && *p1 == *p2) {
    p1++;
    p2++;
  }
  if (n == (size_t)-1) {
    return 0;
  }
  return *p1 - *p2;
}