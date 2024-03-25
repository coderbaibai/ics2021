#include <am.h>
#include <nemu.h>
#include <klib.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)
#define AUDIO_USED_ADDR      (AUDIO_ADDR + 0x18)

void __am_audio_init() {
}
static int buf_size = 0;
void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  buf_size = cfg->bufsize = *(int*)AUDIO_SBUF_SIZE_ADDR;
  cfg->present = 1;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  *(int*)AUDIO_FREQ_ADDR = ctrl->freq;
  *(int*)AUDIO_CHANNELS_ADDR = ctrl->channels;
  *(int*)AUDIO_SAMPLES_ADDR = ctrl->samples;
  *(int*)AUDIO_INIT_ADDR = 1;
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = *(int*)AUDIO_USED_ADDR;
  // stat->count = 0;
  // printf("free:%d\n",buf_size-stat->count);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  int size = (uint8_t*)ctl->buf.end-(uint8_t*)ctl->buf.start;
  // 缓冲区长度不应为0，如果是则再次查询
  if(buf_size==0){
    buf_size = *(int*)AUDIO_SBUF_SIZE_ADDR;
  }
  if(size>buf_size){
    panic("size>buf_size\n");
  }
  volatile int count;
  do
  {
    count = *(int*)AUDIO_COUNT_ADDR;
  } while (size+count>buf_size);
  memcpy((uint8_t*)AUDIO_SBUF_ADDR+count,ctl->buf.start,size);
  // *(int*)AUDIO_COUNT_ADDR += size;
  // printf("memcpy size:%d to %p\n",size,(uint8_t*)AUDIO_SBUF_ADDR+count);
}
