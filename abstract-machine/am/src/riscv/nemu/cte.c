#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

// const char *regs[] = {
//   "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
//   "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
//   "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
//   "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
// };


// void handle_context_display(Context *c){
//   printf("context display\n");
//   for(int i=0;i<32;i++){
//     if(i%2)
//       printf("%3s = 0x%08x\n",regs[i],c->gpr[i]);
//     else
//       printf("%3s = 0x%08x  ",regs[i],c->gpr[i]);
//   }
//   printf("%7s = 0x%08x","mepc   ",c->mepc);
//   printf("%7s = 0x%08x\n","mstatus",c->mstatus);
//   printf("%7s = 0x%08x","mcause ",c->mcause);
// }

Context* __am_irq_handle(Context *c) {
  // handle_context_display(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case -1: ev.event = EVENT_YIELD; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
