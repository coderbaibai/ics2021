#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};


void isa_reg_display() {
  for(int i=0;i<32;i++){
    if(i%2)
      printf("%3s = 0x%08x\n",regs[i],cpu.gpr[i]._32);
    else
      printf("%3s = 0x%08x  ",regs[i],cpu.gpr[i]._32);
  }
}

word_t isa_reg_str2val(const char *s, bool *success) {
  if(strcmp(regs[0],s)==0)
      return cpu.gpr[0]._32;
  for(int i=1;i<32;i++){
    if(strcmp(regs[i],s+1)==0)
      return cpu.gpr[i]._32;
  }
  *success = false;
  return 0;
}
