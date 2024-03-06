extern word_t zero_null;
def_EHelper(jal){
    rtl_addi(s,ddest,&s->pc,4);
    s->dnpc = s->pc+id_src1->imm;
    #ifdef CONFIG_FTRACE
    if(ddest==&cpu.gpr[0]._32){
        fs_pop();
    }
    bool isCall = true;
    char* temp = get_func_name(s->dnpc,&isCall);
    Assert(temp!=NULL,"func not found");
    if(isCall) fs_push(temp,s->dnpc);
    else Assert(strcmp(temp,fs_top()->name)==0,"func not found");
    free(temp);
    #endif
}

def_EHelper(jalr){
    rtl_addi(s,ddest,&s->pc,4);
    s->dnpc = *dsrc1+id_src2->imm;
    #ifdef CONFIG_FTRACE
    bool isCall = true;
    if(ddest==&zero_null){
        fs_pop();
        if(dsrc1==&cpu.gpr[1]._32) return;
        else{
            char* temp = get_func_name(s->dnpc,&isCall);
            Assert(temp!=NULL,"func not found");
            if(isCall) fs_push(temp,s->dnpc);
            else Assert(strcmp(temp,fs_top()->name)==0,"func not found");
            free(temp);
        }
    }
    else{
        char* temp = get_func_name(s->dnpc,&isCall);
        Assert(temp!=NULL,"func not found");
        if(isCall) fs_push(temp,s->dnpc);
        else Assert(strcmp(temp,fs_top()->name)==0,"func not found");
        free(temp);
    }
    #endif
}