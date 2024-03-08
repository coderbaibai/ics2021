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

static inline void uintToStringHex(uint32_t val,char* res){
  if(val==0){
    res[0] = '0';
    res[1] = '\0';
    return;
  }
  int i = 0;
  while(val!=0){
    char t = val%16;
    res[i] = t<10?t+'0':t-10+'a';
    val /= 16;
    i++;
  }
  res[i] = '\0';
  i--;
  int j = 0;
  char t;
  while(i>j){
    t = res[i];
    res[i] = res[j];
    res[j] = t;
    i--;
    j++;
  }
}
static inline void uintToStringdec(uint32_t val,char* res){
  if(val==0){
    res[0] = '0';
    res[1] = '\0';
    return;
  }
  int i = 0;
  while(val!=0){
    res[i] = val%10+'0';
    val /= 10;
    i++;
  }
  res[i] = '\0';
  i--;
  int j = 0;
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
  const char* base = target;
  for(int i=0;i<5;i++)
    switch (i)
    {
    case 0:{
      fmtd.flag = flag_none;
      for(int j=0;j<flag_none;j++){
        if(strncmp(target,flags[j],1)==0){
          fmtd.flag = (Flag)j;
          target++;
          break;
        }
      }
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
      fmtd.precision = 0;
      while(temp!=target){
        fmtd.precision*=10;
        fmtd.precision+=(*temp-'0');
        temp++;
      }
      break;
    }
    case 3:{
      fmtd.length = length_none;
      for(int j=0;j<length_none;j++){
        if(strncmp(target,lengths[j],1)==0){
          fmtd.length = (Length)j;
          target++;
          break;
        }
      }
      break;
    }
    case 4:{
      fmtd.spec = spec_none;
      for(int j=0;j<spec_none-2;j++){
        if(strncmp(target,specs[j],1)==0){
          fmtd.spec = (Specifier)j;
          target++;
          break;
        }
      }
      if(strncmp(target,"lu",2)==0){
          fmtd.spec = lu_sign;
          target+=2;
      }
      else if(strncmp(target,"llu",3)==0){
          fmtd.spec = llu_sign;
          target+=3;
      }
      if(fmtd.spec==spec_none) halt(4);
    }
  }
  fmtd.size = target-base;
  return fmtd;
}

void fillOutString(char** out,const char* in,Fmt_Detail fmtd){
  int space_size = fmtd.width-strlen(in);
  char padding = ' ';
  if(fmtd.flag==zero) padding = '0';
  while(space_size>0){
    **out = padding;
    (*out)++;
    --space_size;
  }
  for(const char* t = in;*t!='\0';t++){
    **out = *t;
    (*out)++;
  }
}

int fmt_to_out(char *out, const char *fmt, va_list va){
  char * p = out;
  const char * q = fmt;
  char * tempString = NULL;
  uint64_t tempValue = 0;
  while(*q!='\0'){
    if(*q=='%'){
      q++;
      Fmt_Detail fmtd = parse_fmt(q);
      q+=fmtd.size;
      switch (fmtd.spec)
      {
        case s_sign:{
          tempString = va_arg(va,char*);
          fillOutString(&p,tempString,fmtd);
          tempString = NULL;
          break;
        }
        case d_sign:{
          char tStr[20];
          tempString = tStr;
          tempValue = va_arg(va,int);
          intToString(tempValue,tempString);
          fillOutString(&p,tempString,fmtd);
          tempString = NULL;
          break;
        }
        case u_sign:{
          char tStr[20];
          tempString = tStr;
          tempValue = va_arg(va,uint32_t);
          uintToStringdec(tempValue,tempString);
          fillOutString(&p,tempString,fmtd);
          tempString = NULL;
          break;
        }
        case x_sign:{
          char tStr[20];
          tempString = tStr;
          tempValue = va_arg(va,uint32_t);
          uintToStringHex(tempValue,tempString);
          fillOutString(&p,tempString,fmtd);
          tempString = NULL;
          break;
        }
        case c_sign:{
          char tStr[20];
          tStr[0] = va_arg(va,int);
          tStr[1] = '\0';
          fillOutString(&p,tStr,fmtd);
          break;
        }
        default: halt(fmtd.spec);
      }
    }
    *p = *q;
    ++p;
    ++q;
  }
  *p = '\0';
  return p-out;
}