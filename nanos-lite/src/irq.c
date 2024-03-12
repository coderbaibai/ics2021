#include <common.h>
#include <am.h>
#include "syscall.h"
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_SYSCALL:{
      switch(c->GPR1){
        case SYS_yield: sys_yield(c); break;
        case SYS_exit: sys_exit(c); break;
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
