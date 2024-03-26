#include<stdio.h>
#include<fixedptc.h>

int main(){
    fixedpt a = fixedpt_rconst(-1.2);
    printf("a floor:%d\n",fixedpt_toint(fixedpt_floor(a)));
    printf("a ceil :%d\n",fixedpt_toint(fixedpt_ceil(a)));
    a = fixedpt_abs(a);
    printf("|a| floor:%d\n",fixedpt_toint(fixedpt_floor(a)));
    printf("|a| ceil :%d\n",fixedpt_toint(fixedpt_ceil(a)));

    fixedpt b = fixedpt_rconst(2.7);
    printf("b floor:%d\n",fixedpt_toint(fixedpt_floor(b)));
    printf("b ceil :%d\n",fixedpt_toint(fixedpt_ceil(b)));
    b = fixedpt_abs(b);
    printf("|b| floor:%d\n",fixedpt_toint(fixedpt_floor(b)));
    printf("|b| ceil :%d\n",fixedpt_toint(fixedpt_ceil(b)));
}