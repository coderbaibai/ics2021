#include<stdio.h>
#include<common.h>
void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *e, bool *success);
void init_regex();
int main(int argc,char**argv){
    init_monitor(argc, argv);
    const size_t numSize = 200;
    uint32_t resReal[numSize];
    char expression[numSize][100];
    for(int pos = 0;pos<numSize;pos++){
        scanf("%u",&resReal[pos]);
        scanf("%s",&expression[pos][0]);
    }
    bool res = true;
    word_t s;
    for(int i=0;i<numSize;i++){
        s = expr(expression[i],&res);
        if(!res||s!=resReal[i]){
            printf("error at %s\n",expression[i]);
            printf("real: %u\n",resReal[i]);
            printf("my result %u\n",s);
            break;
        }
        else{
            printf("success\n");
        }
    }
    return 0;
}