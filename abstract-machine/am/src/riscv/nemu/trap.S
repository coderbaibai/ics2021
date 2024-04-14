#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

#if __riscv_xlen == 32
#define LOAD  lw
#define STORE sw
#define XLEN  4
#else
#define LOAD  ld
#define STORE sd
#define XLEN  8
#endif

#define REGS(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15) f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27) f(28) f(29) \
f(30) f(31)

#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);

#define CONTEXT_SIZE  ((32 + 3 + 1 + 1) * XLEN)
#define OFFSET_SP     ( 2 * XLEN)
#define OFFSET_CAUSE  (32 * XLEN)
#define OFFSET_STATUS (33 * XLEN)
#define OFFSET_EPC    (34 * XLEN)
#define OFFSET_NP     (35 * XLEN)

.align 3
.globl __am_asm_trap
__am_asm_trap:
  # 先判断当前进程属于用户进程还是内核线程(初始状态下，mscratch=0，即为内核主线程)
  csrrw sp,mscratch,sp
  bnez sp,usr_save_context # 如果sp!=0,说明是用户线程；sp=0，说明是内核线程，需要恢复
  csrr sp,mscratch
  j kernel_save_context
usr_save_context:
  addi sp, sp, -CONTEXT_SIZE # 给context预留空间
  
  # 写入context中的SP寄存器，作为用户栈地址
  csrrw t0,mscratch,t0
  STORE t0,OFFSET_SP(sp)
  # 用一个np来标志当前上下文是用户还是内核
  li t0,0
  STORE t0,OFFSET_NP(sp)
  # 为了使得CTE可重入，在中断期间无论如何mscratch都是0，直到下一个用户进程载入为止
  csrrw t0,mscratch,t0
  j common_save_context
kernel_save_context:
  addi sp, sp, -CONTEXT_SIZE # 给context预留空间

  csrrw t0,mscratch,t0
  # 用一个np来标志当前上下文是用户还是内核
  li t0,1
  STORE t0,OFFSET_NP(sp)
  li t0,0
  # 为了使得CTE可重入，在中断期间无论如何mscratch都是0，直到下一个用户进程载入为止
  csrrw t0,mscratch,t0

common_save_context:
  MAP(REGS, PUSH)

  csrr t0, mcause
  csrr t1, mstatus
  csrr t2, mepc

  STORE t0, OFFSET_CAUSE(sp)
  STORE t1, OFFSET_STATUS(sp)
  STORE t2, OFFSET_EPC(sp)

  # set mstatus.MPRV to pass difftest
  li a0, (1 << 17)
  or t1, t1, a0
  csrw mstatus, t1

  mv a0, sp

  jal __am_irq_handle
  # 重新获取sp
  mv sp, a0

  LOAD t1, OFFSET_STATUS(sp)
  LOAD t2, OFFSET_EPC(sp)
  csrw mstatus, t1
  csrw mepc, t2
  
  # 判断新的上下文是用户还是内核,如果是用户的话，需要设置mscratch，并且重新设置SP为用户栈地址
  LOAD t0, OFFSET_NP(sp)
  bnez t0,kernel_restore_context # np为0代表用户进程,非0则跳转

user_restore_context:
  # 将mscratch重新设置为栈底
  addi t0, sp, CONTEXT_SIZE
  csrw mscratch,t0
  MAP(REGS,POP)
  # 重新设置SP为用户栈地址
  LOAD sp,OFFSET_SP(sp)
  j common_restore_context

kernel_restore_context:
  MAP(REGS, POP)

  addi sp, sp, CONTEXT_SIZE

common_restore_context:
  mret
