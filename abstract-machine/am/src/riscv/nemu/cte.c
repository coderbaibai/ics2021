#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};

    ev.event = c->mcause;

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  printf("mepc:%08x\n",c->mepc);
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

// 内核线程的首次初始化，初始化在栈顶
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* res = (Context*)(kstack.end-sizeof(Context));
  memset(res,0,sizeof(Context));
  res->mstatus = 0x1800;
  res->mepc = (uintptr_t)entry;
  res->SP = (uintptr_t)res;
  return res;
}

void yield() {
  asm volatile("li a7, 1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
