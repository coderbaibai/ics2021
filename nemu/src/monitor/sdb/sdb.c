#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
word_t paddr_read(paddr_t addr, int len);
word_t expr(char *e, bool *success);
void watchpoint_display();
void free_wp(int number);
void new_wp(char * wp_expr);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}
static int cmd_si(char *args) {
  uint32_t stepNumber = 1;
  if(args){
    stepNumber = atoi(args);
  }
  if(stepNumber==0){
    printf("Error args:%s\n",args);
    return 0;
  }
  cpu_exec(stepNumber);
  return 0;
}

static int cmd_info(char *args) {
  if(args==NULL){
    printf("No Arg input\n");
  }
  else if(strcmp("r",args)==0){
    isa_reg_display();
  }
  else if(strcmp("w",args)==0){
    watchpoint_display();
  }
  else{
    printf("Error args\n");
  }
  return 0;
}

static int cmd_x(char* args){
  char * end = args+strlen(args);
  char * arg_1 = strtok(args, " ");;
  char * arg_2 = arg_1 + strlen(arg_1)+1;
  char * exprP = NULL;
  int stepNumber = 1;
  paddr_t addr;
  if(!args){
    printf("Error args\n");
    return 0;
  }
  // 如果没有第二个参数
  if(arg_2>=end){
    exprP = arg_1;
  }
  // 如果有第二个参数
  else{
    // 第一个参数一定是个大于1的数
    stepNumber = atoi(arg_1);
    if(stepNumber<=0){
      printf("Error args\n");
      return 0;
    }
    exprP = arg_2;
  }
  // 第一个参数一定是一个表达式
  bool success = true;
  addr = expr(exprP,&success);
  if(!success){
    printf("Error address\n");
  }
  word_t val;
  printf("addr:<0x%08x>\n",addr);
  for(int i=0;i<stepNumber;i++){
    val = paddr_read(addr+i*4,4);
    printf("<0x%08x>: %08x\n",addr+i*4,val);
  }
  return 0;
}
static int cmd_p(char *args){
  if(!args){
    printf("Error args\n");
    return 0;
  }
  bool success = true;
  word_t val = expr(args,&success);
  if(!success){
    printf("Error expression\n");
    return 0;
  }
  printf("%u\n",val);
  return 0;
}
static int cmd_w(char* args){
  if(!args){
    printf("Error args\n");
    return 0;
  }
  new_wp(args);
  return 0;
}

static int cmd_d(char* args){
  if(!args){
    printf("Error args\n");
    return 0;
  }
  if(strlen(args)==1&&args[0]=='0'){
    free_wp(0);
    return 0;
  }
  int wp_number = atoi(args);
  if(wp_number==0){
    printf("Error number\n");
    return 0;
  }
  free_wp(wp_number);
  return 0;
}
static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "exec step by step", cmd_si },
  { "info", "print the status of a program",cmd_info },
  { "x", "print the values of a segement of memory or the register",cmd_x },
  { "p", "print the value of the expression",cmd_p },
  { "w", "add a watchpoint",cmd_w },
  { "d", "delete a watchpoint",cmd_d }

  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
