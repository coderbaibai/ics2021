#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(add) f(sub) f(addi) f(lui) f(auipc)\
    f(xor) f(or) f(and) f(xori) f(ori) f(andi) \
    f(sll) f(srl) f(sra) f(slt) f(sltu) f(slli) f(srli) f(srai) f(slti) f(sltiu) \
    f(lb) f(lh) f(lw) f(lbu) f(lhu) \
    f(sb) f(sh) f(sw) \
    f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) \
    f(jal) f(jalr) \
    f(mul) f(mulh) f(mulhsu) f(mulhu) f(div) f(divu) f(rem) f(remu) \
    f(csrrw) f(csrrs) f(csrrc) f(csrrwi) f(csrrsi) f(csrrci) f(ecall) f(mret)\
    f(inv) f(nemu_trap) \


def_all_EXEC_ID();
