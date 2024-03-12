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
      switch(c->GPR1){
        case SYS_exit: sys_exit(c); break;
        case SYS_yield: sys_yield(c); break;
      }
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
