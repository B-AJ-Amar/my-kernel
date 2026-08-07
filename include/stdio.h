#ifndef STDIO_H
#define STDIO_h

#include <stdarg.h>
#include <stddef.h>

void clear_screen();
int puts(const char *str);
int putchar(char c);
int vsnprintf(char *buf, size_t n, const char *format, va_list ap);
int vprintf(const char *fmt, va_list args);
int printf(const char *format, ...);
void panic(const char *format, ...);
int snprintf(char *buf, size_t n, const char *format, ...);
int sprintf(char *buf, const char *format, ...);

// TODO: add more function likr scanf ....
#endif
