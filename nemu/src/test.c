#include<stdio.h>
#include<common.h>
word_t expr(char *e, bool *success);
int main(){
    bool res = false;
    expr("3 +4*5",&res);
    printf("%d",res);
    return 0;
}