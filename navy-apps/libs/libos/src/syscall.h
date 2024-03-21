#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <sys/time.h>
enum {
  SYS_exit,
  SYS_yield,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};
int sys_yield();
int sys_exit();
int sys_write(int fd, const void* buf,size_t conut);
int sys_brk(void*addr);
int sys_open(const char *pathname, int flags, int mode);
int sys_read(int fd, void *buf, size_t len);
int sys_close(int fd);
int sys_lseek(int fd, size_t offset, int whence);
int sys_gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
