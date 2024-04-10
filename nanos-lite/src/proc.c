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

static inline int getSize(char *const target[]){
  if(!target) return 0;
  for (size_t i = 0; i < INT32_MAX; i++)
  {
    if(target[i]==NULL) return i;
  }
  panic("error arr");
}
void context_uload(PCB*target,const char* fn_name,char *const argv[], char *const envp[]){
  // 初始化传入参数,这是操作系统在创建进程的准备工作之一
  int app_argc = getSize(argv);
  int app_envpc = getSize(envp);
  int init_size = 0,str_area_size = 0;
  init_size+=4*(1+app_argc+1+app_envpc+1);
  for(int i=0;i<app_argc;i++){
    str_area_size += 1+strlen(argv[i]);
  }
  for(int i=0;i<app_envpc;i++){
    str_area_size = 1+strlen(envp[i]);
  }
  // 保证四字节对齐
  str_area_size = 4*(str_area_size/4)+4;
  init_size+=str_area_size;
  void* page_addr = new_page(8);
  int** cur = (int**)((int)page_addr-init_size);
  char* s_cur = (char*)((int)page_addr-str_area_size);
  *((int*)cur) = app_argc;
  cur++;
  for(int i=0;i<app_argc;i++,cur++){
    *cur = (int*)s_cur;
    strcpy(s_cur,argv[i]);
    s_cur+=strlen(argv[i])+1;
  }
  *cur = NULL;
  cur++;
  for(int i=0;i<app_envpc;i++,cur++){
    *cur = (int*)s_cur;
    strcpy(s_cur,envp[i]);
    s_cur+=strlen(envp[i])+1;
  }
  *cur = NULL;
  // 在内核栈中创建上下文
  void* fn = uload(target,fn_name);
  Area kstack;
  kstack.start = target->stack;
  kstack.end = target->stack+sizeof(target->stack);
  target->cp = ucontext(NULL,kstack,fn);
  // 初始化栈顶指针
  target->cp->GPRx = (uintptr_t)((int)page_addr-init_size-1);
}

void init_proc() {
  context_kload(&pcb[0],hello_fun,(void*)0x0);
  // context_kload(&pcb[1],hello_fun,(void*)0x1);
  char* argv[]={"/bin/nterm",NULL};
  char* envp[]={NULL};
  context_uload(&pcb[1],argv[0],argv,envp);
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
