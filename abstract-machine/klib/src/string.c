#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char* t = s;
  for(;*t!='\0';t++);
  return t-s;
}

char *strcpy(char *dst, const char *src) {
  char* tDst = dst;
  const char* tSrc = src;
  while(*tSrc!='\0'){
    *tDst = *tSrc;
    tSrc++;
    tDst++;
  }
  *tDst = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char* tDst = dst;
  const char* tSrc = src;
  int i = 0;
  while(i<n){
    *tDst = *tSrc;
    tSrc++;
    tDst++;
    i++;
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  char* tDst = dst + strlen(dst);
  const char* tSrc = src;
  while(*tSrc!='\0'){
    *tDst = *tSrc;
    tSrc++;
    tDst++;
  }
  tDst = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  for(int i=0;;i++){
    if(s1[i]>s2[i]) {
      return 1;
    }
    else if(s1[i]<s2[i]){
      return -1;
    }
    if(s1[i]=='\0'&&s2[i]=='\0')
        return 0;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  for(int i=0;i<n;i++){
    if(s1[i]>s2[i]) {
      return 1;
    }
    else if(s1[i]<s2[i]){
      return -1;
    }
    if(s1[i]=='\0'&&s2[i]=='\0')
        return 0;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  char* t = s;
  while(n>0){
    *t = (char)c;
    t++;
    n--;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  char* tDst = dst;
  const char* tSrc = src;
  int i = 0;
  while(i<n){
    *tDst = *tSrc;
    tSrc++;
    tDst++;
    i++;
  }
  return dst;
}

inline void *memcpy(void *out, const void *in, size_t n) {
  char* tDst = out;
  const char* tSrc = in;
  for(;n>=4;n-=4,tSrc+=4,tDst+=4){
    (*(uint32_t*)tDst) = (*(uint32_t*)tSrc);
  }
  for(;n>0;n--,tSrc++,tDst++){
    *tDst = *tSrc;
  }
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const char * t1 = s1,*t2 = s2;
  for(int i=0;i<n;i++){
    if(t1[i]>t2[i]) {
      return 1;
    }
    else if(t1[i]<t2[i]){
      return -1;
    }
  }
  return 0;
}

#endif
