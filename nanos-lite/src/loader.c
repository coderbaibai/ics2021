#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;
  ramdisk_read(&elf_header,0,sizeof(Elf_Ehdr));
  Elf_Phdr* p_pheader = (Elf_Phdr*)malloc(elf_header.e_phnum*sizeof(Elf_Phdr));
  ramdisk_read(p_pheader,elf_header.e_phoff,elf_header.e_phnum*sizeof(Elf_Phdr));
  for(int i=0;i<elf_header.e_phnum;i++,p_pheader++){
    if(p_pheader->p_type==PT_LOAD){
      ramdisk_read((void*)p_pheader->p_vaddr,p_pheader->p_offset,p_pheader->p_memsz);
      for(char* t= (char*)(p_pheader->p_vaddr+p_pheader->p_filesz);t<(char*)(p_pheader->p_vaddr+p_pheader->p_memsz);t++){
        *t=0;
        t++;
      }
    }
  }
  free(p_pheader);
  return elf_header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

