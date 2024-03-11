#include <common.h>
#include <am.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD: printf("event yield\n"); break;
    default: printf("event not impl:%d\n",e.event);
  }
  
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
