#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include "memory/paddr.h"

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NEQ,
  TK_DEC,
  TK_HEX,
  TK_REG,
  TK_AND

  /* TODO: Add more token types */

};

extern const char * const regsRegEx;
word_t isa_reg_str2val(const char *s, bool *success);

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},     // spaces
  {"\\+", '+'},          // plus
  {"\\-", '-'},          // minus
  {"\\*", '*'},          // multipy
  {"/", '/'},            // divide
  {"\\(", '('},            // (
  {"\\)", ')'},            // )
  {"\\$[a-zA-Z0-9]+", TK_REG},         // register
  {"==", TK_EQ},         // equal
  {"!=", TK_NEQ},         // not equal
  {"&&", TK_AND},         // and
  {"0x[0-9]+", TK_HEX},// numer_hex
  {"[0-9]+", TK_DEC},  // numer_dec
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  for(int i=0;i<32;i++){
    tokens[i].type = 0;
    memset(tokens[i].str,'\0',32*sizeof(char));
  }
  nr_token = 0;
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  int tokenPos = 0;
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            // i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if(substr_len>32){
          printf("token is longer than 32\n");
          return false;
        }

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;
          case TK_REG:{
            strncpy(tokens[tokenPos].str,substr_start,substr_len);
            bool success = true;
            word_t val = isa_reg_str2val(tokens[tokenPos].str,&success);
            if(!success)
              return false;
            tokens[tokenPos].type = TK_DEC;
            sprintf(tokens[tokenPos].str,"%d",val);
            tokenPos++;
            break;
          } 
          case TK_HEX:{
            strncpy(tokens[tokenPos].str,substr_start,substr_len);
            int temp = 0;
            sscanf(tokens[tokenPos].str,"%x",&temp);
            tokens[tokenPos].type = TK_DEC;
            sprintf(tokens[tokenPos].str,"%d",temp);
            tokenPos++;
            break;
          } 
          case TK_DEC:{
            strncpy(tokens[tokenPos].str,substr_start,substr_len);
            tokens[tokenPos].type = rules[i].token_type;
            tokenPos++;
            break;
          }
          default:
            tokens[tokenPos].type = rules[i].token_type;
            tokenPos++;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  nr_token = tokenPos;

  return true;
}

word_t evaluate(bool *success,int p,int q){
  int res = 0;
  if(p>q){
    *success = false;
    return 0;
  }
  if(p==q){
    if(tokens[p].type!=TK_DEC){
      *success = false;
      return 0;
    }
    sscanf(tokens[p].str,"%d",&res);
    return res;
  }
  else{
    int left = 0,right = 0;
    int lval=0,rval=0;

    for(int i=q;i>=p;i--){
      if(tokens[i].type==TK_AND){
        if(left!=right)
          continue;
        lval = evaluate(success,p,i-1);
        rval = evaluate(success,i+1,q);
        return lval&&rval;
      }
      else if(tokens[i].type=='(') left++;
      else if(tokens[i].type==')') right++;
    }
    if(left!=right){
      *success = false;
      return 0;
    }
    left = right = 0;

    for(int i=q;i>=p;i--){
      if(tokens[i].type==TK_EQ||tokens[i].type==TK_NEQ){
        if(left!=right)
          continue;
        lval = evaluate(success,p,i-1);
        rval = evaluate(success,i+1,q);
        switch (tokens[i].type)
        {
          case TK_EQ:
            return lval==rval;
          case TK_NEQ:
            return lval!=rval;
        }
      }
      else if(tokens[i].type=='(') left++;
      else if(tokens[i].type==')') right++;
    }
    left = right = 0;

    for(int i=q;i>=p;i--){
      if(tokens[i].type=='+'||tokens[i].type=='-'){
        if(left!=right)
          continue;
        lval = evaluate(success,p,i-1);
        rval = evaluate(success,i+1,q);
        switch (tokens[i].type)
        {
          case '+':
            return lval+rval;
          case '-':
            return lval-rval;
        }
      }
      else if(tokens[i].type=='(') left++;
      else if(tokens[i].type==')') right++;
    }
    left = right = 0;

    for(int i=q;i>=p;i--){
      if(tokens[i].type=='*'||tokens[i].type=='/'){
        if(left!=right)
          continue;
        if(i==p)
          continue;
        lval = evaluate(success,p,i-1);
        rval = evaluate(success,i+1,q);
        switch (tokens[i].type)
        {
          case '*':
            return lval*rval;
          case '/':
            if(rval==0){
              *success = false;
              return 0;
            }
            return lval/rval;
        }
      }
      else if(tokens[i].type=='(') left++;
      else if(tokens[i].type==')') right++;
    }
    if(tokens[p].type=='*'){
      rval = (word_t)evaluate(success,p+1,q);
      return paddr_read(rval,4);
    }
    return evaluate(success,p+1,q-1);
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return evaluate(success,0,nr_token-1);

}

