#include <am.h>
#include <nemu.h>

static uint64_t start_time;

void __am_timer_init() {
  uintptr_t p = RTC_ADDR;
  start_time = *(uint64_t*)(p);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uintptr_t p = RTC_ADDR;
  uptime->us = *(uint64_t*)(p) - start_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
