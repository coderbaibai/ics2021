rtlreg_t* getCSR(uint32_t val);
def_EHelper(csrrw){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  rtl_addi(s,t,dsrc1,0);
}
def_EHelper(csrrs){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  rtl_or(s,t,t,dsrc1);
}
def_EHelper(csrrc){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  rtl_and(s,t,t,dsrc1);
}
def_EHelper(csrrwi){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  *t = s->isa.instr.i.rs1;
}
def_EHelper(csrrsi){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  rtl_ori(s,t,t,s->isa.instr.i.rs1);
}
def_EHelper(csrrci){
  rtlreg_t* t = getCSR(id_src2->simm&0xfff);
  rtl_addi(s,ddest,t,0);
  rtl_andi(s,t,t,s->isa.instr.i.rs1);
}

def_EHelper(ecall){
  s->dnpc = isa_raise_intr(cpu.gpr[17]._32, cpu.pc);
}

def_EHelper(mret){
  s->dnpc = cpu.mepc+4;
}