#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
int sys_yield(){
  printf("yeild\n");
  return 0;
}
int sys_exit(){
  return 0;
}
int sys_write(int fd, const void* buf,size_t conut){
  if(fd!=1&&fd!=2){
    return 0;
  }
  const char* cur = buf;
  size_t i = 0;
  while(i<conut){
    putch(*cur);
    cur++;
    i++;
  }
  return i;
}

int sys_brk(void*addr){
  // if((int)addr<0x88000000&&(int)addr>0x80000000){
    printf("addr:%p\n",(int)addr);
    return 0;
  // }
  // return -1;
}