#include <isa.h>

// 这是所谓的中断隐指令
word_t isa_raise_intr(word_t NO, vaddr_t mepc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
#ifdef CONFIG_ETRACE
  log_write("exception: %d\n",NO);
#endif
  cpu.mepc = mepc;
  cpu.mcause = NO;
  cpu.mstatus.MPIE = cpu.mstatus.MIE;
  cpu.mstatus.MIE = 0;
  return cpu.mtvec;
}
#define IRQ_TIMER 0x80000007
word_t isa_query_intr() {
  if(cpu.INTR&&cpu.mstatus.MIE){
    cpu.INTR = false;
    Log("size:%08x\n",sizeof(cpu.mstatus));
    Log("mstatus:%08x MIE:%08x\n",cpu.mstatus.val,cpu.mstatus.MIE);
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}
