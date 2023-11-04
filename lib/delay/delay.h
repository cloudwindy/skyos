#ifndef SKYRADIO_DELAY_H
#define SKYRADIO_DELAY_H

#include <stdint.h>
#include <libopencm3/stm32/timer.h>
#define DELAY_TIMER TIM3

void delay(uint32_t ms);
void udelay(uint32_t us);

#endif