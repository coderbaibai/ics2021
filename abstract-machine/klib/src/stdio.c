#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include "stdio-utils.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


int printf(const char *fmt, ...) {
  va_list args;
  va_start(args,0);
  va_end(args);
  char out[500];
  out[499] = '\0';
  int res = fmt_to_out(out,fmt,args);
  putstr(out);
  return res;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args,0);
  va_end(args);
  return fmt_to_out(out,fmt,args);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
