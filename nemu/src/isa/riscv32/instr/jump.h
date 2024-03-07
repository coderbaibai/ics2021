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
    else {
        char*top_name = fs_top()->name;
        for(int i=0;i<strlen(top_name);i++){
            if(top_name[i]=='@'){
                top_name[i] = '\0';
                Assert(strcmp(temp,fs_top()->name)==0,"temp:%s,top:%s\n",temp,fs_top()->name);
                top_name[i] = '@';
                break;
            }
        }
    }
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
            else {
                char*top_name = fs_top()->name;
                for(int i=0;i<strlen(top_name);i++){
                    if(top_name[i]=='@'){
                        top_name[i] = '\0';
                        Assert(strcmp(temp,fs_top()->name)==0,"temp:%s,top:%s\n",temp,fs_top()->name);
                        top_name[i] = '@';
                        break;
                    }
                }
                Assert(strcmp(temp,fs_top()->name)==0,"temp:%s,top:%s\n",temp,fs_top()->name);
            }
            free(temp);
        }
    }
    else{
        char* temp = get_func_name(s->dnpc,&isCall);
        Assert(temp!=NULL,"func not found");
        if(isCall) fs_push(temp,s->dnpc);
        else {
            char*top_name = fs_top()->name;
            for(int i=0;i<strlen(top_name);i++){
                if(top_name[i]=='@'){
                    top_name[i] = '\0';
                    Assert(strcmp(temp,fs_top()->name)==0,"temp:%s,top:%s\n",temp,fs_top()->name);
                    top_name[i] = '@';
                    break;
                }
            }
            Assert(strcmp(temp,fs_top()->name)==0,"temp:%s,top:%s\n",temp,fs_top()->name);
        }
        free(temp);
    }
    #endif
}