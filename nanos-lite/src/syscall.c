#include <common.h>
#include "syscall.h"
#include <sys/time.h>

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
Context* schedule(Context *prev);
Context* sys_yield(Context *c){
  return schedule(c);
}
int sys_exit(){
  return 0;
}

int sys_brk(void*addr){
  return 0;
}
// 文件操作
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

// 文件系统调用实现
int sys_open(const char *pathname, int flags, int mode){
  return fs_open(pathname,flags,mode);
}

int sys_read(int fd, void *buf, size_t len){
  return fs_read(fd,buf,len);
}

int sys_write(int fd, const void* buf,size_t conut){
  return fs_write(fd,buf,conut);
}
int sys_close(int fd){
  return fs_close(fd);
};
int sys_lseek(int fd, size_t offset, int whence){
  return fs_lseek(fd,offset,whence);
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz){
  if(tv){
    long time = io_read(AM_TIMER_UPTIME).us;
    tv->tv_sec = time/1000000;
    tv->tv_usec = time-tv->tv_sec*1000000;
  }
  return 0;
}
#include <proc.h>
void naive_uload(PCB *pcb, const char *filename);
int isFileExist(const char* filename);
extern PCB *current;
void context_uload(PCB*target,const char* fn_name,char *const argv[], char *const envp[]);
void switch_boot_pcb();

int sys_execve(const char *fname, char * const argv[], char *const envp[]){
  char* input = (char*)malloc(strlen(fname)*sizeof(char));
  strcpy(input,fname);
  for(int i=0;i<strlen(input);i++){
    if(input[i]=='\n'){
      input[i] = '\0';
      break;
    }
  }
  if(isFileExist(input)==0){
    for(int i =0;i<10;i++){
      if(!(envp[i])){
        printf("%d\n",i);
        break;
      }
      else{
        printf("%s\n",envp[i]);
      }
    }
    // naive_uload(NULL,input);
    context_uload(current,input,argv,envp);
    switch_boot_pcb();
    yield();
    return 0;
  }
  else{
    printf("file not exit:%s\n",input);
    return -1;
  }
  free(input);
}