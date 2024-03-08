#include <am.h>
#include <nemu.h>
#include <klib.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
void __am_gpu_config(AM_GPU_CONFIG_T *cfg);
int wid = 0,hei = 0;
void __am_gpu_init() {
  int i;
  AM_GPU_CONFIG_T cfg;
  __am_gpu_config(&cfg);
  int w = cfg.width;
  int h = cfg.height;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 0, .height = 0,
    .vmemsz = 0
  };
  uint32_t res = *(uint32_t*)VGACTL_ADDR;
  wid = cfg->height = res&0xffff;
  hei = cfg->width = (res>>16)&0xffff;
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t *in = ctl->pixels;
  int k = 0;
  for(int j=0;j<hei;j++){
    for(int i=0;i<wid;i++){
      if(i>=ctl->x&&j>=ctl->y){
        fb[k] = in[k];
        k++;
      }
    }
  }
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
