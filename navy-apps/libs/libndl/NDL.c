#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;
static int canvas_position = 0;
uint32_t NDL_GetTicks() {
  struct timeval time;
  gettimeofday(&time,NULL);

  static uint32_t init_time = 0;
  static int is_init = 0;
  if(!is_init){
    init_time = time.tv_sec*1000+time.tv_usec/1000;
    is_init = 1;
  }
  return time.tv_sec*1000+time.tv_usec/1000-init_time;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events",O_RDWR);
  assert(fd!=-1);
  int res = read(fd,buf,len);
  close(fd);
  return res;
}

void NDL_OpenCanvas(int *w, int *h) {
  int fd = open("/proc/dispinfo",O_RDWR);
  assert(fd!=-1);
  char dispinfo[50]={'\0'};
  read(fd,dispinfo,50);
  close(fd);
  int sw,sh;
  sscanf(dispinfo,"WIDTH:%d\nHEIGHT:%d",&sw,&sh);
  if(*w==0&&*h==0){
    *w = sw;
    *h = sh;
  }
  screen_w = sw;
  screen_h = sh;
  canvas_w = *w;
  canvas_h = *h;
  canvas_position = (screen_w-canvas_w)/2 + screen_w*(screen_h-canvas_h)/2;
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
  if(screen_h==0||screen_w==0){
    printf("NDL screen error\n");
    assert(0);
  }
  int fdi = open("/dev/fb",O_WRONLY);
  printf("draw:%d %d %d %d\n",x,y,w,h);
  printf("size:%d %d %d %d\n",screen_h,screen_w,canvas_h,canvas_w);
  uint32_t temp[500];
  assert(fdi!=-1);
  for(int i=y;i<h+y;i++){
    int off = canvas_position+i*screen_w+x;
    lseek(fdi,off,SEEK_SET);
    write(fdi,pixels+(i-y)*w,4*w);
  }
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
