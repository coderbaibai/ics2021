#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

uint8_t cur_state[sizeof(keyname)/sizeof(char*)];

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *event) {
  char buf[20];
  NDL_PollEvent(buf,sizeof(buf));
  memset(cur_state,0,sizeof(cur_state)/sizeof(uint8_t));
  if(strlen(buf)<2) return 0;
  if(strncmp(buf,"kd",2)==0){
    event->type = SDL_KEYDOWN;
  }
  else if(strncmp(buf,"ku",2)==0){
    event->type = SDL_KEYUP;
  }
  else{
    printf("error buf:%s\n",buf);
    return 0;
  }
  for(int i=1;i<sizeof(keyname)/sizeof(char*);i++){
    if(strcmp(&buf[3],keyname[i])==0){
      event->key.keysym.sym = i;
      printf("i:%d %s\n",i,keyname[i]);
      cur_state[i] = 1;
      return 1;
    }
  }
  // printf("no match buf:%s\n",buf);
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  while(!SDL_PollEvent(event));
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return cur_state;
}
