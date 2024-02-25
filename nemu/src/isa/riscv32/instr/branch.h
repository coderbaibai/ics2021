def_EHelper(beq) {
  rtl_setrelop(s,RELOP_EQ,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}

def_EHelper(bne) {
  rtl_setrelop(s,RELOP_NE,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}

def_EHelper(blt) {
  rtl_setrelop(s,RELOP_LT,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}

def_EHelper(bge) {
  rtl_setrelop(s,RELOP_GE,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}

def_EHelper(bltu) {
  rtl_setrelop(s,RELOP_LTU,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}

def_EHelper(bgeu) {
  rtl_setrelop(s,RELOP_GEU,t0,dsrc1,dsrc2);
  if(*t0){
    s->dnpc = s->pc+id_dest->imm;
  }
}