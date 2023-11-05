#include "pwm.h"
#include "delay.h"

#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

/**
 * PWM 启用波形发生器
 */
void pwm_init(const PwmGenerator *pwm_gen)
{
  gpio_set_mode(pwm_gen->gpio_port, GPIO_MODE_OUTPUT_2_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, pwm_gen->gpios);
}

/**
 * PWM 实时生成波形
 *
 * freq   Frequency in Hertz
 * length Total length in Microseconds
 * cycle
 */
void pwm_gen_rt(const PwmGenerator *pwm_gen, uint32_t freq, uint32_t length, double cycle)
{
  if (cycle == 0)
  {
    cycle = pwm_gen->default_cycle;
  }
  double period_us = 1000000 / freq;
  double total_duration = 0;
  while (total_duration / 1000 <= length)
  {
    total_duration += period_us;
    gpio_set(pwm_gen->gpio_port, pwm_gen->gpios);
    udelay(period_us * cycle);
    gpio_clear(pwm_gen->gpio_port, pwm_gen->gpios);
    udelay(period_us * (1 - cycle));
  }
}