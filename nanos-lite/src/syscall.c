#include <common.h>
#include "syscall.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
int sys_yield(){
  printf("yeild\n");
  return 0;
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
  // 对于标准输入输出的处理
  if(fd<3&&fd>=0)
    panic("not impl");
  printf("fd:%d\n",fd);
  return fs_read(fd,buf,len);
}

int sys_write(int fd, const void* buf,size_t conut){
  // 对于标准输入输出的处理
  if(fd<3&&fd>=0){
    const char* cur = buf;
    size_t i = 0;
    while(i<conut){
      putch(*cur);
      cur++;
      i++;
    }
    return i;
  }
  return fs_write(fd,buf,conut);
}
int sys_close(int fd){
  return fs_close(fd);
};
int sys_lseek(int fd, size_t offset, int whence){
  // 对于标准输入输出的处理
  if(fd<3&&fd>=0){
    return -1;
  }
  return fs_lseek(fd,offset,whence);
}