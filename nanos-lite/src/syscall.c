#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
void sys_yield(Context* c){
  printf("yeild\n");
  c->GPRx = 0;
}
void sys_exit(Context* c){
  halt(0);
}