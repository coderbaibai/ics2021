#include "sdb.h"
#include "isa.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char * wp_expr;
  word_t val;
  /* TODO: Add more members if necessary */
} WP;

word_t expr(char *e, bool *success);

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
extern CPU_state cpu;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].wp_expr = NULL;
  }

  head = NULL;
  free_ = wp_pool;
}

void new_wp(char * wp_expr){
  Assert(free_!=NULL,"no free watch points");
  bool success = true;
  word_t value = expr(wp_expr,&success);
  if(!success){
    printf("error expression\n");
    return;
  }
  if(head==NULL){
    head = free_;
    head->next = NULL;
    free_ = free_->next;
    head->wp_expr =wp_expr;
    head->val = value;
    printf("hardware watchpoint %d: %s\n",head->NO,wp_expr);
    return;
  }
  else{
    WP* cur = head;
    while(cur->next!=NULL){
      cur = cur->next;
    }
    cur->next = free_;
    free_ = free_->next;
    cur = cur->next;
    cur->next = NULL;
    cur->wp_expr = wp_expr;
    cur->val = value;
    printf("Hardware watchpoint %d: %s\n",cur->NO,wp_expr);
    return;
  }
}

void free_wp(int number){
  if(number>=NR_WP){
    printf("no match wp in using\n");
    return;
  }
  WP* cur = head;
  while(cur->next!=NULL){
    if(cur->next->NO==number){
      WP* temp = cur->next->next;
      cur->next->next = free_;
      free_ = cur->next;
      cur->next = temp;
      return;
    }
  }
  printf("no match wp in using\n");
  return;
}

void watchpoint_display(){
  WP* cur = head;
  if(cur==NULL){
    printf("no watchpoint yet\n");
    return;
  }
  printf("NO    expr\n");
  while(cur!=NULL){
    printf("%-6d%s\n",cur->NO,cur->wp_expr);
    cur = cur->next;
  }
}
void prompt_if_change(){
  WP* cur = head;
  uint32_t newVal;
  bool success = true;
  while(cur!=NULL){
    newVal = expr(cur->wp_expr,&  success);
    Assert(success,"expr error:%s",cur->wp_expr);
    if(newVal!=cur->val){
      printf("Hardware watchpoint: %d: %s\n",cur->NO,cur->wp_expr);
      printf("Old value = %u\n",cur->val);
      printf("New value = %u\n",newVal);
      printf("pc at <0x%08x>\n",cpu.pc);
      cur->val = newVal;
      nemu_state.state = NEMU_STOP;
    }
    cur = cur->next;
  }
}
/* TODO: Implement the functionality of watchpoint */

