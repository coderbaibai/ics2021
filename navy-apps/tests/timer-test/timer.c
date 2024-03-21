#include<sys/time.h>
#include<stdio.h>

int main(){
    struct timeval cur_time;
    gettimeofday(&cur_time,NULL);
    long uptime = cur_time.tv_usec/1000;
    while(1){
        gettimeofday(&cur_time,NULL);
        printf("cur time usecond:%d\n",cur_time.tv_usec/1000);
        if(cur_time.tv_usec/1000<uptime) continue;
        printf("0.5 second pass\n");
        uptime+=500;
    }
    return 0;
}