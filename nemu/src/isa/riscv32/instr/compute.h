def_EHelper(add){
  rtl_add(s,ddest,dsrc1,dsrc2);
}

def_EHelper(sub){
  rtl_sub(s,ddest,dsrc1,dsrc2);
}

def_EHelper(addi){
  rtl_addi(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  rtl_addi(s,ddest,&s->pc,id_src1->imm);
}

def_EHelper(xor) {
  rtl_xor(s,ddest,dsrc1,dsrc2);
}

def_EHelper(or) {
  rtl_or(s,ddest,dsrc1,dsrc2);
}

def_EHelper(and) {
  rtl_and(s,ddest,dsrc1,dsrc2);
}

def_EHelper(xori) {
  rtl_xori(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(ori) {
  rtl_ori(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(andi) {
  rtl_andi(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(sll) {
  rtl_sll(s,ddest,dsrc1,dsrc2);
}

def_EHelper(srl) {
  rtl_srl(s,ddest,dsrc1,dsrc2);
}

def_EHelper(sra) {
  rtl_sra(s,ddest,dsrc1,dsrc2);
}

def_EHelper(slt) {
  rtl_setrelop(s,RELOP_LT,ddest,dsrc1,dsrc2);
}

def_EHelper(sltu) {
  rtl_setrelop(s,RELOP_LTU,ddest,dsrc1,dsrc2);
}

def_EHelper(slli) {
  rtl_slli(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(srli) {
  rtl_srli(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(srai) {
  rtl_srai(s,ddest,dsrc1,id_src2->simm);
}

def_EHelper(slti) {
  rtl_setrelopi(s,RELOP_LT,ddest,dsrc1,id_src2->simm);
}

def_EHelper(sltiu) {
  rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1,id_src2->simm);
}

def_EHelper(mul) {
  rtl_mulu_lo(s,ddest,dsrc1,dsrc2);
}

def_EHelper(mulh) {
  rtl_muls_hi(s,ddest,dsrc1,dsrc2);
}

def_EHelper(mulhsu) {
  rtl_mulsu_hi(s,ddest,dsrc1,dsrc2);
}

def_EHelper(mulhu) {
  rtl_mulu_hi(s,ddest,dsrc1,dsrc2);
}

def_EHelper(div) {
  rtl_divs_q(s,ddest,dsrc1,dsrc2);
}

def_EHelper(divu) {
  rtl_divu_q(s,ddest,dsrc1,dsrc2);
}

def_EHelper(rem) {
  rtl_divs_r(s,ddest,dsrc1,dsrc2);
}

def_EHelper(remu) {
  rtl_divu_r(s,ddest,dsrc1,dsrc2);
}