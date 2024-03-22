#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  const char* cur = buf;
  size_t i = 0;
  while(i<len){
    putch(*cur);
    cur++;
    i++;
  }
  return 0;
}

size_t serial_read(void *buf, size_t offset, size_t len){
  panic("serial_read has no implement");
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  char str[20] = {'\0'};
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  strcat(str,ev.keydown?"kd ":"ku ");
  strcat(str,keyname[ev.keycode]);
  strcat(str,"\n");
  strncpy((char*)buf,str,len);
  if(ev.keycode==AM_KEY_NONE) return 0;
  return len<strlen((char*)buf)?len:strlen((char*)buf);
}

size_t events_write(const void *buf, size_t offset, size_t len){
  panic("event_write has no implement");
  return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T gpu_config = io_read(AM_GPU_CONFIG);
  char info[50];
  sprintf(info,"WIDTH:%d\nHEIGHT:%d\n",gpu_config.width,gpu_config.height);
  printf("info:%d",gpu_config.width);
  memcpy(buf,info,len);
  return 0;
}

size_t dispinfo_write(const void *buf, size_t offset, size_t len){
  panic("dispinfo_write has no implement");
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
