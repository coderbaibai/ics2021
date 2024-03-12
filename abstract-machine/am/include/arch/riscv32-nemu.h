#ifndef ARCH_H__
#define ARCH_H__
#include<stdint.h>
struct Context {
  // TODO: fix the order of these members to match trap.S
  uintptr_t info,gpr[31], mcause, mstatus,mepc;
  void *pdir;
};

#define GPR1 gpr[16] // a7
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[9]

#endif
