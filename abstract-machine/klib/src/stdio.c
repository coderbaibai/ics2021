#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#if defined(__ISA_X86__)||defined(__ISA_MIPS32__)||defined(__ISA_RISCV32__)
# define ADDR_BIT 32
#elif defined(__ISA_RISCV64__) || defined(__ISA_NATIVE__)
# define ADDR_BIT 64
#elif 1
123456
#endif

#include "stdio-utils.h"

int printf(const char *fmt, ...) {
  char out[4096];
  va_list args;
  va_start(args,fmt);
  int res = fmt_to_out(out,fmt,args);
  out[4095] = '\0';
  assert(res<4096);
  va_end(args);
  putstr(out);
  return res;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args,fmt);
  int res = fmt_to_out(out,fmt,args);
  va_end(args);
  return res;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
