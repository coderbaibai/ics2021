#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uintptr_t cur = KBD_ADDR;
  kbd->keycode = *(uint32_t*)cur;
  if(kbd->keycode>AM_KEY_NONE&&kbd->keycode<=AM_KEY_PAGEDOWN){
    kbd->keydown = 1;
  }
  else{
    kbd->keycode=AM_KEY_NONE;
    kbd->keydown = 0;
  }
}
