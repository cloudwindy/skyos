#include "setup.h"
#include "led.h"
#include "delay.h"
#include "pwm.h"
#include "keypad.h"
#include "serial.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libopencm3/stm32/iwdg.h>

static void test_tx(void);

int main(void)
{
  setup();
  (void)test_tx;
  char last_char = 0;
  char *serial_buf;
  serial_buf = malloc(64);
  while (true)
  {
    /**
     * 主循环
     */
    char new_char = keypad_getchar();
    if (new_char != last_char && new_char != 0)
    {
      printf("%c", new_char);
    }
    int len = serial_recv(serial_buf);
    if (len > 0)
    {
      led_blink();
      printf("%s", serial_buf);
      memset(serial_buf, 0, 64);
    }
    last_char = new_char;
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

  while (true)
  {
    gpio_set(GPIOB, GPIO1);
    delay(3);
    gpio_clear(GPIOB, GPIO1);
    delay(3);
  }
  // pwm_gen_rt(&gen, 1200, 1000000, 0);
  // printf("Font TX\n");
  // for (size_t i = 0; i < 1520; i++)
  // {
  //   iwdg_reset();
  //   pwm_gen_rt(&gen, 1000, 5, 0);
  //   pwm_send(&gen, Font.data[i], 5, 5);
  //   printf("Byte %i TX\n", i + 1);
  // }
  printf("Done\n");
}