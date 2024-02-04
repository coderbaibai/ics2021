#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n) {
    uint32_t random_num = (uint32_t)rand();
    uint32_t max_value = RAND_MAX - (RAND_MAX % n);
    
    while (random_num >= max_value) {
        random_num = (uint32_t)rand();
    }
    
    return random_num % n;
}

static char* gen_num(){
  char* res = (char*)malloc(11*sizeof(char));
  memset(res, '\0', 11 * sizeof(char));
  sprintf(res,"%u",choose(100));
  return res;
}

static char* gen_rand_expr(uint32_t* times) {
  char* res = (char*)malloc(65536*sizeof(char));
  memset(res, '\0', 65536 * sizeof(char));
  char* tempPointer = NULL;
  if(*times==32||*times==31)
    return gen_num();
  switch (choose(6))
  { 
  case 0:
    return gen_num();
    break;
  case 1:
    *times+=2;
    tempPointer = gen_rand_expr(times);
    strcpy(res,tempPointer);
    free(tempPointer);
    res[strlen(res)] = '+';
    tempPointer = gen_rand_expr(times);
    strcpy(&res[strlen(res)],tempPointer);
    free(tempPointer);
    return res;
  case 2:
    *times+=2;
    tempPointer = gen_rand_expr(times);
    strcpy(res,tempPointer);
    free(tempPointer);
    res[strlen(res)] = '-';
    tempPointer = gen_rand_expr(times);
    strcpy(&res[strlen(res)],tempPointer);
    free(tempPointer);
    return res;
  case 3:
    *times+=2;
    tempPointer = gen_rand_expr(times);
    strcpy(res,tempPointer);
    free(tempPointer);
    res[strlen(res)] = '*';
    tempPointer = gen_rand_expr(times);
    strcpy(&res[strlen(res)],tempPointer);
    free(tempPointer);
    return res;
  case 4:
    *times+=2;
    tempPointer = gen_rand_expr(times);
    strcpy(res,tempPointer);
    free(tempPointer);
    res[strlen(res)] = '/';
    tempPointer = gen_rand_expr(times);
    strcpy(&res[strlen(res)],tempPointer);
    free(tempPointer);
    return res;
  case 5:
    *times+=2;
    res[0] = '(';
    tempPointer = gen_rand_expr(times);
    strcpy(&res[1],tempPointer);
    free(tempPointer);
    res[strlen(res)] = ')';
    return res;
  } 
}

static void gen_rand_expr_outer(){
  uint32_t times = 1;
  strcpy(buf,gen_rand_expr(&times));
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr_outer();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -Werror -o /tmp/.expr>/dev/null 2>&1");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
