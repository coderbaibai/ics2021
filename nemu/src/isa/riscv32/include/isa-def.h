#ifndef __ISA_RISCV32_H__
#define __ISA_RISCV32_H__

#include <common.h>

typedef union
{
  rtlreg_t val;
  struct{
    uint32_t UIE:1;
    uint32_t SIE:1;
    uint32_t ZERO_0:1;
    uint32_t MIE:1;
    uint32_t UPIE:1;
    uint32_t SPIE:1;
    uint32_t ZERO_1:1;
    uint32_t MPIE:1;
    uint32_t SPP:1;
    uint32_t ZERO_2:2;
    uint32_t MPP:2;
    uint32_t FS:2;
    uint32_t XS:2;
    uint32_t MPRV:1;
    uint32_t SUM:1;
    uint32_t MXR:1;
    uint32_t TVM:1;
    uint32_t TW:1;
    uint32_t TSR:1;
    uint32_t ZERO_3:8;
    uint32_t SD:1;
  };
}reg_mstatus;


typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];
  vaddr_t mtvec;
  vaddr_t mepc;
  reg_mstatus mstatus;
  rtlreg_t mcause;
  rtlreg_t satp;
  rtlreg_t mscratch;
  vaddr_t pc;
  bool INTR;
} riscv32_CPU_state;

// decode
typedef struct {
  union {
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t funct7    : 7;
    } r;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      int32_t  simm11_0  :12;
    } i;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm4_0    : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  simm11_5  : 7;
    } s;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm11     : 1;
      uint32_t imm4_1    : 4;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t imm10_5  : 6;
      int32_t  simm12    : 1;
    } b;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm31_12  :20;
    } u;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm19_12  : 8;
      uint32_t imm11     : 1;
      uint32_t imm10_1   :10;
      int32_t  simm20     : 1;
    } j;
    uint32_t val;
  } instr;
} riscv32_ISADecodeInfo;

// #define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
