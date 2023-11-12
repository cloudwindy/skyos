#ifndef LED_H
#define LED_H

#include <libopencm3/stm32/gpio.h>

#define LED_DELAY 5
#define LED_BANK GPIOC
#define LED GPIO13

void setup_led(void);
void led_blink(void);
void led_on(void);
void led_off(void);
void led_pwm_dimming(double brightness);

#endif