#include "led.h"
#include "delay.h"

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