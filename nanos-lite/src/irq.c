#include <common.h>
#include <am.h>
#include<sys/time.h>
#include "syscall.h"
#include "autoconf.h"
typedef struct FRec
{
  int fd;
  const char* filename;
  const char* opname;
}FRec;

static void setFRec(FRec* frec,int fd,const char* filename,const char* opname){
  frec->fd = fd;
  frec->filename = filename;
  frec->opname = opname;
}
char* getFilenameById(int i);
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_SYSCALL:{
      int ret = 0;
      FRec frec={-1,NULL,NULL};
      switch(c->GPR1){
        case SYS_exit : ret = sys_exit(c); break;
        case SYS_yield: ret = sys_yield(c); break;
        case SYS_open : 
          ret = sys_open((const char*)c->GPR2,(int)c->GPR3,(int)c->GPR4);
          setFRec(&frec,ret,(const char*)c->GPR2,"open"); 
          break;
        case SYS_read : 
          ret = sys_read((int)c->GPR2,(void*)c->GPR3,(size_t)c->GPR4);
          setFRec(&frec,(int)c->GPR2,NULL,"read"); 
          break;
        case SYS_write: 
          ret = sys_write((int)c->GPR2,(const void*)c->GPR3,(size_t)c->GPR4);
          setFRec(&frec,(int)c->GPR2,NULL,"write");
          break;
        case SYS_close: 
          ret = sys_close((int)c->GPR2);
          setFRec(&frec,(int)c->GPR2,NULL,"close");
          break;
        case SYS_lseek: 
          ret = sys_lseek((int)c->GPR2,(size_t)c->GPR3,(int)c->GPR4); 
          setFRec(&frec,(int)c->GPR2,NULL,"lseek");
          break;
        case SYS_brk  : ret = sys_brk((void*)c->GPR2); break;
        case SYS_gettimeofday: ret = sys_gettimeofday((struct timeval*)c->GPR2,(struct timezone*)c->GPR3); break;
        default: panic("syscall not impl:%d\n",c->GPR1);
      }
      #ifdef CONFIG_STRACE
      if(frec.fd!=-1){
        if(frec.filename==NULL) frec.filename = getFilenameById(frec.fd);
        printf("%5s %s\n",frec.opname,frec.filename);
        setFRec(&frec,-1,NULL,NULL);
      }
      else printf("syscall number:%d\n",c->GPR1);
      #endif
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
