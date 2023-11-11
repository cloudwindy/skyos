#include "delay.h"

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>

// 定时器: 间隔
static uint32_t interval_val = 0;
// 定时器: 回调
static void (*interval_cb_ptr)(void);
// 计时器: 剩余时间
static volatile uint32_t timer_val = 0;

/**
 * 启用定时器
 */
void set_interval(uint32_t ms, void (*callback)(void))
{
  timer_val = ms;
  interval_val = ms;
  interval_cb_ptr = callback;
}

uint32_t get_interval(void)
{
  return interval_val;
}

uint32_t delay_get_val(void)
{
  return timer_val;
}

/**
 * 计时
 */
void delay(uint32_t ms)
{
  timer_val = ms;

  while (timer_val > 0)
    ;
}

/**
 * 精确计时
 */
void udelay(uint32_t us)
{
  timer_set_counter(DELAY_PRECISE_TIMER, 0);
  while (timer_get_counter(DELAY_PRECISE_TIMER) < us)
    ;
}

void sys_tick_handler(void)
{
  if (timer_val > 0)
  {
    timer_val--;
  }
  else if (timer_val == 0 && interval_val > 0)
  {
    timer_val = 0;
    interval_cb_ptr();
    // 重置定时器
    timer_val = interval_val;
  }
}