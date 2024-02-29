#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void intToString(int val,char* res){
  if(val==0){
    res[0] = '0';
    res[1] = '\0';
    return;
  }
  int negtive = 0;
  int i = 0;
  if(val<0){
    res[0] = '-';
    val = -val;
    negtive = 1;
  }
  while(val!=0){
    res[i] = val%10;
    val /= 10;
    i++;
  }
  res[i] = '\0';
  i--;
  int j = negtive?1:0;
  char t;
  while(i>j){
    t = res[i];
    res[i] = res[j];
    res[j] = t;
    i--;
    j++;
  }
}

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
      switch (*(++q))
      {
        case 's':{
          tempString = va_arg(va,char*);
          for(char* t = tempString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          tempString = NULL;
          ++q;
          continue;
        }
        case 'd':{
          tempInteger = va_arg(va,int);
          intToString(tempInteger,aidString);
          for(char* t = aidString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          ++q;
          continue;
        }
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
