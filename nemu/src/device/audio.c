#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  reg_used,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
static SDL_AudioSpec s ={};
static int cur = 0;
pthread_mutex_t audio_config_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t audio_buf_mutex = PTHREAD_MUTEX_INITIALIZER;

void audio_cb(void* udata, Uint8* stream, int len){
  SDL_memset(stream,0,len);
  // 如果剩余长度为0，则直接返回
  // 获取锁
  pthread_mutex_lock(&audio_buf_mutex);
  pthread_mutex_lock(&audio_config_mutex);
  if(cur>=audio_base[reg_count]){
    pthread_mutex_unlock(&audio_config_mutex);
    pthread_mutex_unlock(&audio_buf_mutex);
    return;
  }
  // 返回剩余长度和len中的较小者，将这么多的数据放入
  int relen = audio_base[reg_count]-cur<len?audio_base[reg_count]-cur:len;
  SDL_MixAudio(stream, &sbuf[cur], relen, SDL_MIX_MAXVOLUME);
  // SDL_memcpy(stream, &sbuf[cur], relen);
  cur+=relen;
  audio_base[reg_used] = audio_base[reg_count]-cur;
  if(cur==audio_base[reg_count]){
    cur = audio_base[reg_count] = 0;
  }
  // printf("cb: cur   is %d\n",cur);
  // printf("cb: count is %d\n",audio_base[reg_count]);
  pthread_mutex_unlock(&audio_config_mutex);
  pthread_mutex_unlock(&audio_buf_mutex);
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  if(offset%4!=0) return;
  audio_base[reg_used] = audio_base[reg_count]-cur;
  switch (offset/4)
  {
    case reg_init:
    if(audio_base[reg_init]==1){
      s.freq = audio_base[reg_freq];
      s.channels = audio_base[reg_channels];
      s.samples = audio_base[reg_samples];
      s.format = AUDIO_S16SYS;
      s.userdata = NULL;
      s.callback = audio_cb;
      SDL_InitSubSystem(SDL_INIT_AUDIO);
      SDL_OpenAudio(&s, NULL);
      SDL_PauseAudio(0);
    }
    break;
  
  default:
    break;
  }
}
static void audio_buf_handler(uint32_t offset, int len, bool is_write) {
  pthread_mutex_lock  (&audio_config_mutex);
  audio_base[reg_count]+=len;
  audio_base[reg_used] = audio_base[reg_count]-cur;
  pthread_mutex_unlock(&audio_config_mutex);
}


void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler,&audio_config_mutex);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, audio_buf_handler,&audio_buf_mutex);

  memset(sbuf,0,CONFIG_SB_SIZE);
  audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
  audio_base[reg_count] = 0;
  audio_base[reg_used] = 0;
}
