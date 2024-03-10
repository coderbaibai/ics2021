#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t mepc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.mepc = mepc;
  cpu.mcause = NO;
  return cpu.mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
