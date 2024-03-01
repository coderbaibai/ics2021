#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
extern long img_size;
void pc_around_instrs_display(uint32_t size){
// 获取边界虚拟地址
  vaddr_t posBefore = cpu.pc-size*4>RESET_VECTOR?cpu.pc-size*4:RESET_VECTOR;
  vaddr_t posAfter = cpu.pc+size*4<RESET_VECTOR+img_size/2?cpu.pc+size*4:RESET_VECTOR+img_size/2;
//  获取长度 
  uint32_t sizeBefore = cpu.pc-posBefore/4+1;
  uint32_t sizeAfter = posAfter-cpu.pc/4;
  char** bufBefore = (char**) malloc(sizeof(char*)*sizeBefore);
  char** bufAfter = (char**) malloc(sizeof(char*)*sizeAfter);
  int j = 0;
  for(char** buf = bufBefore;j<sizeBefore;buf++,j++,posBefore+=4){
    *buf = (char*)malloc(sizeof(char)*128);
    char* p = *buf;
    p += snprintf(p, 128, FMT_WORD ":", cpu.pc);
    int ilen = 4;
    int i;
    uint32_t instr_total = vaddr_ifetch(posBefore,4);
    uint8_t *instr = (uint8_t *)&instr_total;
    for (i = 0; i < ilen; i ++) {
        p += snprintf(p, 4, " %02x", instr[i]);
    }
    memset(p, ' ', 1);
    p += 1;
    void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, *buf + sizeof(128) - p,
      cpu.pc, (uint8_t *)&instr_total, ilen);
  }
  j = 0;
  for(char** buf = bufAfter;j<sizeAfter;buf++,j++,posAfter-=4){
    *buf = (char*)malloc(sizeof(char)*128);
    char* p = *buf;
    p += snprintf(p, 128, FMT_WORD ":", cpu.pc);
    int ilen = 4;
    int i;
    uint32_t instr_total = vaddr_ifetch(posBefore,4);
    uint8_t *instr = (uint8_t *)&instr_total;
    for (i = 0; i < ilen; i ++) {
        p += snprintf(p, 4, " %02x", instr[i]);
    }
    memset(p, ' ', 1);
    p += 1;
    void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, *buf + sizeof(128) - p,
      cpu.pc, (uint8_t *)&instr_total, ilen);
  }
  for(char**buf = bufBefore,j=0;j<sizeBefore;j++,buf++){
    printf("   ""%s",*buf);
    free(*buf);
  }
  free(bufBefore);
  for(char**buf = bufAfter+sizeBefore-1,j=0;j<sizeBefore;j++,buf--){
    printf("   ""%s",*buf);
    free(*buf);
  }
  free(bufAfter);
}