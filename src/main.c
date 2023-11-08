#include "setup.h"
#include "led.h"
#include "delay.h"
#include "ssd1306.h"
#include "pwm.h"

#include <stdio.h>
#include <string.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>


int main(void)
{
  setup();
  led_off();

  PwmGenerator gen = {
    .gpio_port = GPIOB,
    .gpios = GPIO1,
    .low_freq = 1200,
    .high_freq = 2200,
    .default_cycle = .5,
  };
  pwm_init(&gen);

  printf("Font TX\n");
  for (size_t i = 0; i < 1520; i++)
  {
    pwm_gen_rt(&gen, 1000, 30, 0);
    delay(10);
    pwm_send(&gen, Font.data[i], 20, 10);
  }
  printf("Done\n");
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
}