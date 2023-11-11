#include "setup.h"
#include "led.h"
#include "delay.h"
#include "pwm.h"
#include "tty.h"
#include "keypad.h"

#include <stdio.h>
#include <string.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/cm3/nvic.h>


static void test_tx(void);

int main(void)
{
  setup();
  (void)test_tx;
  while (true)
  {
    /**
     * 主循环
     */
    char ch = keypad_getchar();
    printf(&ch);
    fflush(stdout);
    iwdg_reset();
    delay(10);
  }
}

static void test_tx(void)
{
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
    pwm_gen_rt(&gen, 1000, 1, 0);
    pwm_send(&gen, Font.data[i], 1, 1);
    printf("Byte %i TX\n", i + 1);
  }
  printf("Done\n");
}

/**
 * RTC 计时器中断
 *
 * 每秒执行一次
 */
void rtc_isr(void)
{
  rtc_clear_flag(RTC_SEC);
  fflush(stdout);
}