#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
int fs_open(const char *pathname, int flags, int mode);

static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename,0,0);
  Elf_Ehdr elf_header;
  fs_read(fd,&elf_header,sizeof(Elf_Ehdr));
  Elf_Phdr* base = (Elf_Phdr*)malloc(elf_header.e_phnum*sizeof(Elf_Phdr));
  Elf_Phdr* p_pheader = base;
  fs_lseek(fd,elf_header.e_phoff,SEEK_SET);
  fs_read(fd,p_pheader,elf_header.e_phnum*sizeof(Elf_Phdr));
  for(int i=0;i<elf_header.e_phnum;i++,p_pheader++){
    if(p_pheader->p_type==PT_LOAD){
      // 程序段起点的块内偏移量
      size_t off = p_pheader->p_vaddr&0xfff;
      // 将目标载入到可执行文件中指定的虚拟地址
      fs_lseek(fd,p_pheader->p_offset-off,SEEK_SET);

      #ifdef HAS_VME
      int cur_size = 0;
      void* ps;
      while(cur_size<p_pheader->p_memsz+off){
        ps = new_page(1);
        fs_read(fd,ps,PGSIZE);
        map(&pcb->as,(char*)p_pheader->p_vaddr+cur_size,ps,0);
        printf("load: map %08x to %08x\n",(char*)p_pheader->p_vaddr+cur_size-off,ps);
        cur_size+=PGSIZE;
        // 将.bss节清零
        if(cur_size-off>p_pheader->p_filesz){
          size_t t_off = p_pheader->p_filesz-cur_size+off+PGSIZE>0?p_pheader->p_filesz-cur_size+off+PGSIZE:0;
          printf("t from %08x to %08x\n",(char*)ps+t_off,(char*)ps+PGSIZE);
          printf("t_off:%d\n",t_off);
          for(char* t = (char*)ps+t_off;t<(char*)ps+PGSIZE;t++){
            *t = 0;
          }
        }
      }
      #else
      fs_read(fd,(void*)p_pheader->p_vaddr,p_pheader->p_memsz);
      for(char* t= (char*)(p_pheader->p_vaddr+p_pheader->p_filesz);t<(char*)(p_pheader->p_vaddr+p_pheader->p_memsz);t++) *t=0;
      #endif
    }
  }
  fs_close(fd);
  free(base);
  printf("load: entry vaddr %08x\n",elf_header.e_entry);
  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void* uload(PCB *pcb,const char *filename){
  uintptr_t entry = loader(pcb, filename);
  return (void*)entry;
}

