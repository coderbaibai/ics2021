#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
typedef union
{
  struct{
    uint32_t PPN_1 :12;
    uint32_t PPN_0 :10;
    uint32_t RSW   : 2;
    uint32_t D     : 1;
    uint32_t A     : 1;
    uint32_t G     : 1;
    uint32_t U     : 1;
    uint32_t X     : 1;
    uint32_t W     : 1;
    uint32_t R     : 1;
    uint32_t V     : 1;
  };
  uint32_t pte;
}PTE;

int isa_mmu_check(vaddr_t vaddr, int len, int type){
  if(cpu.satp&0x80000000) return MMU_TRANSLATE;
  else return MMU_DIRECT;
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  paddr_t pgt_addr = cpu.satp<<12;
  // 找到页目录项
  PTE pte_outer,pte_inner;
  pte_outer.pte = paddr_read(pgt_addr+4*((uint32_t)vaddr>>22),4);
  if(!pte_outer.pte||!pte_outer.V){
    printf("pgt_addr:%08x\n",pgt_addr);
    panic("outer:page fault: access vaddr:%08x",vaddr);
  }
  pte_inner.pte = paddr_read((pte_outer.PPN_1<<22|pte_outer.PPN_0<<12|0)+4*((uint32_t)vaddr>>12&0x3ff),4);
  if(!pte_outer.pte||!pte_outer.V){
    panic("inner:page fault: access vaddr:%08x",vaddr);
  }
  return pte_inner.PPN_1<<22|pte_inner.PPN_0<<12|(vaddr&0x00000fff);
}
