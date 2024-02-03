#include<stdio.h>
#include<common.h>
word_t expr(char *e, bool *success);
void init_regex();
int main(){
    bool res = false;
    init_regex();
    expr("3 +4*5",&res);
    printf("%d",res);
    return 0;
}