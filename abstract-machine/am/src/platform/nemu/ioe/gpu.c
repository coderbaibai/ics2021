#include <am.h>
#include <nemu.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
void __am_gpu_config(AM_GPU_CONFIG_T *cfg);
int wid = 0,hei = 0;
void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 0, .height = 0,
    .vmemsz = 0
  };
  uint32_t res = *(uint32_t*)VGACTL_ADDR;
  hei = cfg->height = res&0xffff;
  wid = cfg->width = (res>>16)&0xffff;
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *in = ctl->pixels;
  for(int j=ctl->y;j<ctl->y+ctl->h;j++){
    memcpy(&fb[j*wid+ctl->x],in,ctl->w*4);
    in+=ctl->w;
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_clear(AM_GPU_CLEAR_T*){
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  memset(fb,0,hei*wid*4);
  outl(SYNC_ADDR, 1);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
