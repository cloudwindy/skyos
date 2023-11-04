#include "setup.h"
#include "led.h"
#include "delay.h"

#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

int main(void)
{
  setup();
  led_off();
  printf("Yes\n");
  while (true)
  {
    printf("It works!");
    delay(100);
    printf("\n");
  }
}
