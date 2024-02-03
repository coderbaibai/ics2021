#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
word_t paddr_read(paddr_t addr, int len);

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
  else{
    printf("Error args\n");
  }
  return 0;
}

static int cmd_x(char* args){
  char * arg_1 = args;
  char * arg_2 = args + strlen(arg_1)+1;
  char * expr = NULL;
  int stepNumber = 1;
  paddr_t addr;
  if(!args){
    printf("Error args\n");
    return 0;
  } 
  // 如果没有第二个参数
  if(!arg_2){
    expr = arg_1;
  }
  // 如果有第二个参数
  else{
    // 第一个参数一定是个大于1的数
    stepNumber = atoi(args);
    if(stepNumber<=0){
      printf("Error args\n");
      return 0;
    }
    expr = arg_2;
  }
  // 第一个参数一定是一个表达式
  if(expr[0]!='0'||expr[1]!='x'){
    printf("Error args\n");
    return 0;
  }
  sscanf(expr,"%x",&addr);
  word_t val;
  for(int i=0;i<stepNumber;i++){
    val = paddr_read(addr+i*4,4);
    printf("<0x%08x>: %x\n",addr+i*4,val);
  }
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
  { "x", "print the values of a segement of memory or the register",cmd_x }

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
