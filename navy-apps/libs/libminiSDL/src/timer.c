#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param) {
  printf("add timer\n");
  return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id) {
  printf("remove timer\n");
  return 1;
}

uint32_t SDL_GetTicks() {
  return NDL_GetTicks();
}

void SDL_Delay(uint32_t ms) {
  uint32_t begin;
  uint32_t cur;
  cur = begin = NDL_GetTicks();
  while(cur-begin<ms){
    cur = NDL_GetTicks();
  }
}
