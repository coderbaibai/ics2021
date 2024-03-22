#include<sys/time.h>
#include<stdio.h>
#include<NDL.h>

int main(){
    long uptime = NDL_GetTicks();
    while(1){
        if(NDL_GetTicks()<uptime) continue;
        printf("0.5 second pass\n");
        uptime+=500;
    }
    return 0;
}