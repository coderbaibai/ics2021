#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include "stdio-utils.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  char * p = out;
  const char * q = fmt;
  char * tempString = NULL;
  char aidString[20];
  int tempInteger = 0;
  va_list va;
  va_start(va,fmt);
  while(*q!='\0'){
    if(*q=='%'){
      q++;
      Fmt_Detail fmtd = parse_fmt(q);
      switch (fmtd.spec)
      {
        case s_sign:{
          tempString = va_arg(va,char*);
          int space_size = fmtd.width-strlen(tempString);
          char padding = ' ';
          if(fmtd.flag==zero) padding = '0';
          while(space_size>0){
            *p = padding;
            ++p;
            --space_size;
          }
          for(char* t = tempString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          tempString = NULL;
          q+=fmtd.size;
          continue;
        }
        case d_sign:{
          tempInteger = va_arg(va,int);
          intToString(tempInteger,aidString);
          int space_size = fmtd.width-strlen(aidString);
          char padding = ' ';
          if(fmtd.flag==zero) padding = '0';
          while(space_size>0){
            *p = padding;
            ++p;
            --space_size;
          }
          for(char* t = aidString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          q+=fmtd.size;
          continue;
        }
        default: panic("sign not impl");
      }
    }
    *p = *q;
    ++p;
    ++q;
  }
  *p = '\0';
  return p-out;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
