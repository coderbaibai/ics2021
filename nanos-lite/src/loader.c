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
      // 将目标载入到可执行文件 中指定的虚拟地址
      fs_lseek(fd,p_pheader->p_offset,SEEK_SET);
      fs_read(fd,(void*)p_pheader->p_vaddr,p_pheader->p_memsz);
      for(char* t= (char*)(p_pheader->p_vaddr+p_pheader->p_filesz);t<(char*)(p_pheader->p_vaddr+p_pheader->p_memsz);t++) *t=0;
    }
  }
  fs_close(fd);
  free(base);
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

