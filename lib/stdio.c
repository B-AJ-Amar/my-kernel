#include <io.h>
#include <kernel/console.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#define MAX_BUFFER_SIZE 2096

static int _utoa(char *buf, size_t buf_size, uint64_t val, int base,
                 int uppercase);

// TODO: add colors support e.g `\033[0;31m`

void clear_screen() { console_clear(); }

int puts(const char *str) {
  if (!str)
    return -1;
  console_write(str);
  console_putchar('\n');
  return 0;
}

int putchar(char c) {
  console_putchar(c);
  return (int)c;
}

int sprintf(char *buf, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int len = vsnprintf(buf, MAX_BUFFER_SIZE, format, args);
  va_end(args);
  return len;
}

int snprintf(char *buf, size_t n, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int len = vsnprintf(buf, n, format, args);
  va_end(args);
  return len;
}

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = vprintf(fmt, args);
  va_end(args);
  return len;
}

void panic(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = vprintf(fmt, args);
  va_end(args);
  hlt();
}

int vprintf(const char *fmt, va_list args) {
  char buffer[MAX_BUFFER_SIZE];

  int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

  console_write(buffer);

  return len;
}
int vsnprintf(char *buf, size_t n, const char *format, va_list ap) {
  size_t total_written = 0;

  if (!format) {
    return -1;
  }

  while (*format) {
    if (*format == '%') {
      format++; // Move past '%'

      // --- length modifier parsing ---
      int long_count = 0;
      while (*format == 'l') {
        long_count++;
        format++;
      }

      if (*format == '%') {
        if (buf && total_written < n) {
          buf[total_written] = '%';
        }
        total_written++;
        format++;
        continue;
      }

      if (*format == 'c') {
        char c = (char)va_arg(ap, int);
        if (buf && total_written < n) {
          buf[total_written] = c;
        }
        total_written++;
      } else if (*format == 's') {
        char *s = va_arg(ap, char *);
        if (!s)
          s = "(null)";

        while (*s) {
          if (buf && total_written < n) {
            buf[total_written] = *s;
          }
          total_written++;
          s++;
        }
      } else if (*format == 'd' || *format == 'i') {
        size_t remaining = (buf && total_written < n) ? (n - total_written) : 0;
        char *target_buf = buf ? &buf[total_written] : NULL;

        int64_t val = (long_count == 2)   ? va_arg(ap, long long)
                      : (long_count == 1) ? va_arg(ap, long)
                                          : va_arg(ap, int);
        if (val < 0) {
          if (buf && total_written < n) {
            buf[total_written] = '-';
          }
          total_written++;
          val = -val;
          remaining = (buf && total_written < n) ? (n - total_written) : 0;
          target_buf = buf ? &buf[total_written] : NULL;
        }
        total_written += _utoa(target_buf, remaining, (uint64_t)val, 10, 0);
      } else if (*format == 'u') {
        size_t remaining = (buf && total_written < n) ? (n - total_written) : 0;
        char *target_buf = buf ? &buf[total_written] : NULL;

        uint64_t val = (long_count == 2)   ? va_arg(ap, unsigned long long)
                       : (long_count == 1) ? va_arg(ap, unsigned long)
                                           : va_arg(ap, unsigned int);
        total_written += _utoa(target_buf, remaining, val, 10, 0);
      } else if (*format == 'x' || *format == 'X') {
        int upper = (*format == 'X');
        size_t remaining = (buf && total_written < n) ? (n - total_written) : 0;
        char *target_buf = buf ? &buf[total_written] : NULL;

        uint64_t val = (long_count == 2)   ? va_arg(ap, unsigned long long)
                       : (long_count == 1) ? va_arg(ap, unsigned long)
                                           : va_arg(ap, unsigned int);
        total_written += _utoa(target_buf, remaining, val, 16, upper);
      } else {
        if (buf && total_written < n) {
          buf[total_written] = *format;
        }
        total_written++;
      }
    } else {
      if (buf && total_written < n) {
        buf[total_written] = *format;
      }
      total_written++;
    }
    format++;
  }

  if (buf && n > 0) {
    if (total_written < n) {
      buf[total_written] = '\0';
    } else {
      buf[n - 1] = '\0';
    }
  }

  return (int)total_written;
}

int _utoa(char *buf, size_t buf_size, uint64_t val, int base, int uppercase) {
  char tmp[64];
  int i = 0;
  int written = 0;

  if (val == 0) {
    tmp[i++] = '0';
  } else {
    while (val > 0) {
      int rem = (int)(val % (uint64_t)base);
      if (rem < 10) {
        tmp[i++] = '0' + rem;
      } else {
        tmp[i++] = (uppercase ? 'A' : 'a') + (rem - 10);
      }
      val /= (uint64_t)base;
    }
  }

  for (int j = i - 1; j >= 0; j--) {
    if (written < (int)buf_size) {
      buf[written] = tmp[j];
    }
    written++;
  }

  return written;
}