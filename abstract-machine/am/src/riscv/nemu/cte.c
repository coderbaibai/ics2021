#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>


#define IRQ_TIMER 0x80000007

static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);
Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    __am_get_cur_as(c);
    switch (c->mcause)
    {
    case EVENT_YIELD:
      ev.event = EVENT_YIELD;
      break;
    case EVENT_SYSCALL:
      ev.event = EVENT_SYSCALL;
      break;
    case IRQ_TIMER:
      ev.event = EVENT_IRQ_TIMER;
      break;
    default:
      ev.event = c->mcause;
      break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
    int a=0x888;
    printf("before:%08x %08x\n",&a,a);
    __am_switch(c);
    printf("after: %08x %08x\n",&a,a);
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

// 内核线程的首次初始化，初始化在栈顶
Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context* res = (Context*)(kstack.end-sizeof(Context));
  memset(res,0,sizeof(Context));
  res->mstatus = 0x1880;
  res->mepc = ((uintptr_t)entry)-sizeof(uintptr_t);
  res->SP = (uintptr_t)res;
  res->GPR2 = (uintptr_t)arg;
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
