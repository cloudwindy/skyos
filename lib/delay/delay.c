#include "delay.h"

#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

static volatile uint32_t delay_left = 0;

void delay(uint32_t ms)
{
  delay_left = ms;
  while (delay_left > 0)
    ;
}

void udelay(uint32_t us)
{
  timer_set_counter(DELAY_TIMER, 0);
  while (timer_get_counter(DELAY_TIMER) < us)
    ;
}

void sys_tick_handler(void)
{
  if (delay_left > 0)
    delay_left--;
}