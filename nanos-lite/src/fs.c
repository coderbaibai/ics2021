#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode){
  for(int i=0;i<sizeof(file_table)/sizeof(Finfo);i++){
    if(strcmp(file_table[i].name,pathname)==0){
      // TODO: 实现文件的状态管理
      // 初始化位置为0
      file_table[i].open_offset = 0;
      // 直接返回文件描述符
      return i;
    }
  }
  // 文件未找到
  panic("file path [%s] not found\n",pathname);
}
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t fs_read(int fd, void *buf, size_t len){
  // 文件描述符不能超过表的范围，否则认为是fault
  assert(fd<sizeof(file_table)/sizeof(file_table[0])&&fd>2);
  // 读取长度不能超过文件的范围，否则返回0
  if(file_table[fd].open_offset+len>file_table[fd].size)
    return 0;
  // 从ramdisk中读取
  ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset += len;
  return len;
}

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_write(int fd, const void *buf, size_t len){
  // 文件描述符不能超过表的范围,且是文件，否则认为是fault
  assert(fd<sizeof(file_table)/sizeof(Finfo)&&fd>2);
  // 写入长度不能超过文件的范围，否则返回0
  if(file_table[fd].open_offset+len>file_table[fd].size)
    return 0;
  // 从ramdisk中读取
  ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset += len;
  return len;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  // 文件描述符不能超过表的范围，否则认为是fault
  assert(fd<sizeof(file_table)/sizeof(Finfo)&&fd>2);

  size_t pos;
  switch (whence)
  {
  case SEEK_SET:
    pos = offset;
    break;
  case SEEK_CUR:
    pos = file_table[fd].open_offset+offset;
    break;
  case SEEK_END:
    pos = file_table[fd].size+offset;
    break;
  // 如果选项不是其中之一
  default:
    panic("error whence\n");
    break;
  }
  // 如果越界
  if(pos>file_table[fd].size||pos<0){
    return -1;
  }
  file_table[fd].open_offset = pos;
  return 0;
}

int fs_close(int fd){
  return 0;
}