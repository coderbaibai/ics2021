#include <klib.h>
int strncmp(const char *s1, const char *s2, size_t n);
static inline void intToString(int val,char* res){
  if(val==0){
    res[0] = '0';
    res[1] = '\0';
    return;
  }
  int negtive = 0;
  int i = 0;
  if(val<0){
    res[0] = '-';
    val = -val;
    negtive = 1;
  }
  while(val!=0){
    res[i] = val%10+'0';
    val /= 10;
    i++;
  }
  res[i] = '\0';
  i--;
  int j = negtive?1:0;
  char t;
  while(i>j){
    t = res[i];
    res[i] = res[j];
    res[j] = t;
    i--;
    j++;
  }
}

typedef enum Flag{
  negtive,
  positive,
  space,
  pound_key,
  zero,
  flag_none
}Flag;

static char* flags[5]={
    "-","+"," ","#","0"
};

typedef enum Length{
  h_sign,
  l_sign,
  L_sign,
  length_none
}Length;

static char* lengths[3]={
    "h","l","L"
};

typedef enum Specifier{
  a_sign,
  d_sign,
  o_sign,
  x_sign,
  u_sign,
  f_sign,
  e_sign,
  g_sign,
  c_sign,
  s_sign,
  p_sign,
  lu_sign,
  llu_sign,
  spec_none
}Specifier;

static char* specs[13]={
    "a","d","o","x","u","f","e","g","c","s","p","lu","llu"
};

typedef struct Fmt_Detail
{
  Flag flag;
  int width;
  Length length;
  int precision;
  Specifier spec;
  int size;
} Fmt_Detail;

static inline Fmt_Detail parse_fmt(const char*target){
  Fmt_Detail fmtd;
  char* base;
  for(int i=0;i<5;i++)
    switch (i)
    {
    case 0:{
      for(int i=0;i<flag_none;i++){
        if(strncmp(target,flags[i],1)==0){
          fmtd.flag = (Flag)i;
          target++;
          break;
        }
      }
      fmtd.flag = flag_none;
      break;
    }
    case 1:{
      fmtd.width = -1;
      const char* temp = target;
      while(*target<'9'&&*target>'0'){
        target++;
      }
      if(target!=temp){
        fmtd.width = 0;
        while(temp!=target){
          fmtd.width*=10;
          fmtd.width+=(*temp-'0');
          temp++;
        }
      }
      break;
    }
    case 2:{
      fmtd.precision = -1;
      if(*target!='.') break;
      target++;
      const char* temp = target;
      while(*target<'9'&&*target>'0'){
        target++;
      }
      assert(temp!=target);
      fmtd.precision = 0;
      while(temp!=target){
        fmtd.precision*=10;
        fmtd.precision+=(*temp-'0');
        temp++;
      }
      break;
    }
    case 3:{
      for(int i=0;i<length_none;i++){
        if(strncmp(target,lengths[i],1)==0){
          fmtd.length = (Length)i;
          target++;
          break;
        }
      }
      fmtd.flag = flag_none;
      break;
    }
    case 4:{
      for(int i=0;i<spec_none-2;i++){
        if(strncmp(target,specs[i],1)==0){
          fmtd.spec = (Specifier)i;
          target++;
          break;
        }
      }
      if(strncmp(target,"lu",2)==0){
          fmtd.spec = lu_sign;
          target+=2;
          break;
      }
      if(strncmp(target,"llu",3)==0){
          fmtd.spec = llu_sign;
          target+=3;
          break;
      }
      assert(0);
    }
  }
  fmtd.size = target-base;
  return fmtd;
}

int fmt_to_out(char *out, const char *fmt, ...){
  char * p = out;
  const char * q = fmt;
  char * tempString = NULL;
  char aidString[20];
  int tempInteger = 0;
  va_list va;
  va_start(va,fmt);
  va_end(va);
  while(*q!='\0'){
    if(*q=='%'){
      q++;
      Fmt_Detail fmtd = parse_fmt(q);
      switch (fmtd.spec)
      {
        case s_sign:{
          tempString = va_arg(va,char*);
          int space_size = fmtd.width-strlen(tempString);
          char padding = ' ';
          if(fmtd.flag==zero) padding = '0';
          while(space_size>0){
            *p = padding;
            ++p;
            --space_size;
          }
          for(char* t = tempString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          tempString = NULL;
          q+=fmtd.size;
          continue;
        }
        case d_sign:{
          tempInteger = va_arg(va,int);
          intToString(tempInteger,aidString);
          int space_size = fmtd.width-strlen(aidString);
          char padding = ' ';
          if(fmtd.flag==zero) padding = '0';
          while(space_size>0){
            *p = padding;
            ++p;
            --space_size;
          }
          for(char* t = aidString;*t!='\0';t++){
            *p = *t;
            ++p;
          }
          q+=fmtd.size;
          continue;
        }
        default: panic("sign not impl");
      }
    }
    *p = *q;
    ++p;
    ++q;
  }
  *p = '\0';
  return p-out;
}