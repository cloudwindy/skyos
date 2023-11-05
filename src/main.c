#include "setup.h"
#include "led.h"
#include "delay.h"

#include <stdio.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>

int main(void)
{
  setup();
  led_off();
  printf("STM32 Sky Radio\n");
  printf("Start up...");

  while (true)
  {
  }
}

/**
 * RTC 计时器中断
 *
 * 每秒执行一次
 */
void rtc_isr(void)
{
  rtc_clear_flag(RTC_SEC);
  printf("\n");
  printf("Time: %lu", rtc_get_counter_val());
}