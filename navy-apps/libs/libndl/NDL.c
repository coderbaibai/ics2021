#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include<fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval time;
  gettimeofday(&time,NULL);
  return time.tv_usec/1000;
}

int NDL_PollEvent(char *buf, int len) {
  FILE* file = fopen("/dev/event","r+");
  return fread(buf,1,len,file);
}

void NDL_OpenCanvas(int *w, int *h) {
  FILE* fd = fopen("/proc/dispinfo","r+");
  char dispinfo[50]={'\0'};
  fread(dispinfo,1,50,fd);
  int sw,sh;
  sscanf(dispinfo,"WIDTH:%d\nHEIGHT:%d",&sw,&sh);
  if(*w==*h==0){
    *w = sw;
    *h = sh;
  }
  screen_w = sw;
  screen_h = sh;
  canvas_w = *w;
  canvas_h = *h;
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
}
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  if(screen_h==0||screen_w==0||canvas_h==0||canvas_w==0){
    printf("NDL screen error\n");
    assert(0);
  }
  FILE* fd = fopen("/dev/fb","r+");
  int fdi = open("/dev/fb",0);
  for(int i=y;i<h+y;i++){
    int off = i*screen_w+x;
    fseek(fd,off,SEEK_SET);
    fwrite(pixels+(i-y)*w,1,4*w,fd);
  }
  fclose(fd);
  write(fdi,pixels,0);
  close(fdi);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
