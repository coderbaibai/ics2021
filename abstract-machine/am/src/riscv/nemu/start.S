.section entry, "ax"
.globl _start
.type _start, @function

_start:
  mv s0, zero
  la sp, _stack_pointer
  beqz a0,_start_init
  mv sp,a0
_start_init:
  jal _trm_init
