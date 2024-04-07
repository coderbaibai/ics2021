#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
void naive_uload(PCB *pcb, const char *filename);
void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    // Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}
// 用于初始化一个内核线程,
void context_kload(PCB*target,void(fn)(void*),void*args){
// 将PCB分配给内核线程
// 也就是给分配栈，并且将线程上下文存入PCB中的cp寄存器
// 在这种设计下，内核线程的栈在PCB之中
// 因此一个PCB可以完全代表一个内核线程，这个栈我们称之为内核栈
  Area kstack;
  kstack.start = target->stack;
  kstack.end = target->stack+sizeof(target->stack);
  target->cp = kcontext(kstack,fn,args);
}


void* uload(PCB *pcb,const char *filename);

void context_uload(PCB*target,const char* fn_name){
  void* fn = uload(target,fn_name);
  Area kstack;
  kstack.start = target->stack;
  kstack.end = target->stack+sizeof(target->stack);
  target->cp = ucontext(NULL,kstack,fn);
  target->cp->GPRx = (uintptr_t)heap.end;
  printf("heap end:%p\n",heap.end);
}

void init_proc() {
  context_kload(&pcb[0],hello_fun,(void*)0x0);
  // context_kload(&pcb[1],hello_fun,(void*)0x1);
  context_uload(&pcb[1], "/bin/pal");
  switch_boot_pcb();

  // Log("Initializing processes...");
  // naive_uload(NULL,"/bin/nterm");
  // load program here
}

Context* schedule(Context *prev) {
  static int cur = -1;
  current->cp = prev;
  if(cur!=0){
    current = &pcb[0];
    cur = 0;
  }
  else{
    cur = 1;
    current = &pcb[1];
  }
  return current->cp;
}
