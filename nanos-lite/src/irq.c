#include <common.h>
#include <am.h>
#include "syscall.h"
#include "autoconf.h"
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_SYSCALL:{
      #ifdef CONFIG_STRACE
      printf("syscall number:%d\n",c->GPR1);
      #endif
      int ret = 0;
      switch(c->GPR1){
        case SYS_exit : ret = sys_exit(c); break;
        case SYS_yield: ret = sys_yield(c); break;
        case SYS_open : ret = sys_open((const char*)c->GPR2,(int)c->GPR3,(int)c->GPR4); break;
        case SYS_read : ret = sys_read((int)c->GPR2,(void*)c->GPR3,(size_t)c->GPR4); break;
        case SYS_write: ret = sys_write((int)c->GPR2,(const void*)c->GPR3,(size_t)c->GPR4); break;
        case SYS_lseek: ret = sys_lseek((int)c->GPR2,(size_t)c->GPR3,(int)c->GPR4); break;
        case SYS_close: ret = sys_close((int)c->GPR1); break;
        case SYS_brk  : ret = sys_brk((void*)c->GPR2); break;
        default: panic("syscall not impl:%d\n",c->GPR1);
      }
      c->GPRx = ret;
      break;
    }
    default: printf("event not impl:%d\n",e.event);
  }
  
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
