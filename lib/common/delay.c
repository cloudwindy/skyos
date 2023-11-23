#include "delay.h"

#include "FreeRTOS.h"
#include "FreeRTOS/task.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/timer.h>

#define DELAY_PRECISE_TIMER (TIM3)

/**
 * Wrapper for vTaskDelay.
 * 
 * Do not use this in setup functions.
 */
void os_delay(uint32_t ms)
{
  vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * Sleep using precise timer.
 */
void sleep(uint32_t ms)
{
  for (uint32_t i = 0; i < ms; i++)
    usleep(1000);
}

/**
 * Sleep using precise timer.
 */
void usleep(uint16_t us)
{
  timer_set_counter(DELAY_PRECISE_TIMER, 0);
  while (timer_get_counter(DELAY_PRECISE_TIMER) < us)
    ;
}
