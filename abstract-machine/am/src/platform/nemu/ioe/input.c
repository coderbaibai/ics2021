#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t res = *(uint32_t*)KBD_ADDR;
  kbd->keycode = res&0xff;
  kbd->keydown = res&KEYDOWN_MASK;
}
