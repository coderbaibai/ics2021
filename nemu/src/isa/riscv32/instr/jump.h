extern word_t zero_null;
def_EHelper(jal){
    rtl_addi(s,ddest,&s->pc,4);
    s->dnpc = s->pc+id_src1->imm;
    #ifdef CONFIG_FTRACE
    
    bool isCall = true;
    char* temp = get_func_name(s->dnpc,&isCall);
    Assert(temp!=NULL,"func not found");
    // 判断是否是函数调用
    // 是函数调用
    if(isCall) {
        // 是尾调用
        if(ddest==&zero_null){
            fs_pop();
        }
        fs_push(temp,s->dnpc);
    }
    // 不是函数调用，需要判断是否与栈顶函数名一致
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
    s->dnpc = *dsrc1+id_src2->imm;
    rtl_addi(s,ddest,&s->pc,4);
    #ifdef CONFIG_FTRACE
    bool isCall = true;
    // 可能是尾调用、函数返回、函数内跳转
    if(ddest==&zero_null){
        // 说明是函数返回指令
        if(dsrc1==&cpu.gpr[1]._32){
            fs_pop();
            return;
        } 
        else{
            char* temp = get_func_name(s->dnpc,&isCall);
            Assert(temp!=NULL,"func not found");
            // 是函数尾调用
            if(isCall) {
                fs_pop();
                fs_push(temp,s->dnpc);
            }
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
        }
    }
    // 可能是函数调用、函数内部跳转
    else{
        char* temp = get_func_name(s->dnpc,&isCall);
        Assert(temp!=NULL,"func not found");
        // 是函数调用，入栈
        if(isCall) {
            fs_push(temp,s->dnpc);
        }
        // 是函数内部跳转，检查
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
    }
    #endif
}