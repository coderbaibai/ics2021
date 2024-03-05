extern word_t zero_null;
def_EHelper(jal){
    rtl_addi(s,ddest,&s->pc,4);
    s->dnpc = s->pc+id_src1->imm;
    #ifdef CONFIG_FTRACE
    if(ddest==&cpu.gpr[0]._32){
        fs_pop();
    }
    char* temp = get_func_name(s->dnpc);
    Assert(temp!=NULL,"func not found");
    fs_push(temp,s->dnpc);
    free(temp);
    #endif
}

def_EHelper(jalr){
    rtl_addi(s,ddest,&s->pc,4);
    s->dnpc = *dsrc1+id_src2->imm;
    #ifdef CONFIG_FTRACE
    if(ddest==&zero_null){
        fs_pop();
        if(dsrc1==&cpu.gpr[1]._32) return;
        else{
            char* temp = get_func_name(s->dnpc);
            Assert(temp!=NULL,"func not found");
            fs_push(temp,s->dnpc);
            free(temp);
        }
    }
    else{
        char* temp = get_func_name(s->dnpc);
        Assert(temp!=NULL,"func not found");
        fs_push(temp,s->dnpc);
        free(temp);
    }
    #endif
}