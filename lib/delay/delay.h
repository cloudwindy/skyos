#ifndef SKYRADIO_DELAY_H
#define SKYRADIO_DELAY_H

#include <stdint.h>
#include <libopencm3/stm32/timer.h>

#define DELAY_PRECISE_TIMER (TIM3)

typedef void(*interval_cb_func) (void);

void delay(uint32_t ms);
uint32_t delay_get_val(void);
void udelay(uint32_t us);
void set_interval(uint32_t ms, void (*callback)(void));
uint32_t get_interval(void);

#endif