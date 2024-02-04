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
    bool res = true;
    word_t s = expr("3-4",&res);
    printf("%u\n",s);
    return 0;
}