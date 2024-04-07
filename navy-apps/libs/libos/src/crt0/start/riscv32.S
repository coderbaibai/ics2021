.globl  _start
_start:
  move s0, zero
  beqz a0,_start_init
  mv sp,a0
_start_init:
  jal call_main
