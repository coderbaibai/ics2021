#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static void sh_handle_cmd(const char *cmd) {
  char* argv[10];
  for(int i=0;i<10;i++){
    argv[i] = NULL;
  }
  int cnt = 0;
  int last = 0;
  for(int i=0;i<=strlen(cmd);i++){
    if(cmd[i]==' '||cmd[i]=='\0'){
      if(i==last||cmd[last]==' '||cmd[last]=='\0'||cmd[last]==10) {last++;continue;}
      argv[cnt] = (char*)malloc(sizeof(char)*(i-last)+1);
      strncpy(argv[cnt],&cmd[last],i-last);
      argv[cnt][i-last] = '\0';
      last = i+1;
      cnt++;
    }
  }
  // debug
  // for(int i=0;i<10;i++){
  //   printf("%s\n",argv[i]);
  // }
  char*temp[10]={"--skip",NULL};
  execvp("pal",temp);
  // if(execvp(argv[0],&argv[1])){
  //   sh_printf("exective file not be found\n");
  // }
  for(int i=0;i<10;i++){
    if(argv[i]) free(argv[i]);
  }
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
