#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  int checked = isa_mmu_check(addr,len,0);
  switch (checked)
  {
  case MMU_DIRECT:
    return paddr_read(addr, len);
    break;
  case MMU_TRANSLATE:
    return paddr_read(isa_mmu_translate(addr,len,0), len);
    break;
  default:
    panic("mmu error");
    break;
  }
  return 0;
}

word_t vaddr_read(vaddr_t addr, int len) {
  int checked = isa_mmu_check(addr,len,0);
  switch (checked)
  {
  case MMU_DIRECT:
    return paddr_read(addr, len);
    break;
  case MMU_TRANSLATE:
    return paddr_read(isa_mmu_translate(addr,len,0), len);
    break;
  default:
    panic("mmu error");
    break;
  }
  return 0;
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  int checked = isa_mmu_check(addr,len,0);
  switch (checked)
  {
  case MMU_DIRECT:
    paddr_write(addr, len, data);
    break;
  case MMU_TRANSLATE:
    paddr_write(isa_mmu_translate(addr,len,0), len, data);
    break;
  default:
    panic("mmu error");
    break;
  }
}
