#include "led.h"
#include "delay.h"
#include "pwm.h"

PwmGenerator led_pwm_gen = {
  .gpio_port = LED_BANK,
  .gpios = LED,
};

void led_blink(void)
{
  led_on();
  delay(LED_DELAY);
  led_off();
  delay(LED_DELAY);
}

void led_on(void)
{
  gpio_clear(LED_BANK, LED);
}

void led_off(void)
{
  gpio_set(LED_BANK, LED);
}

void led_pwm_dimming(double brightness)
{
  pwm_gen_rt(&led_pwm_gen, 200, 1000, brightness);
}