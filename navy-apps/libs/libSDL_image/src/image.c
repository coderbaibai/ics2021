#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);
  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
  // 1、用libc的文件操作打开文件，并获得size
  FILE* file = fopen(filename,"r+");
  assert(file);
  fseek(file,0,SEEK_END);
  int size = ftell(file);
  // 2、申请一段大小为size的内存空间buf
  char* buf = (char*)malloc(size);
  // 3、将整个文件读入buf中
  fseek(file,0,SEEK_SET);
  fread(buf,size,1,file);
  // 4、调用stb库，解析图片
  SDL_Surface* res = STBIMG_LoadFromMemory(buf,size);
  // 关闭文件，释放内存
  fclose(file);
  free(buf);
  return res;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
