#include "pwm.h"
#include "delay.h"

#include <stdio.h>
#include <libopencm3/stm32/gpio.h>

/**
 * PWM 启用波形发生器
 */
void pwm_init(const PwmGenerator *pwm_gen)
{
  gpio_set_mode(pwm_gen->gpio_port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, pwm_gen->gpios);
}

/**
 * PWM 输出一个字节
 */
void pwm_send(const PwmGenerator *pwm_gen, uint8_t byte, uint32_t length_of_bit, uint32_t length_between_bit)
{
  // 逐位输出
  for (int i = 0; i < 8; i++)
  {
    pwm_gen_rt(pwm_gen, byte & 1 ? pwm_gen->high_freq : pwm_gen->low_freq,
               length_of_bit, 0);
    byte = byte >> 1;
    delay(length_between_bit);
  }
}

/**
 * PWM 实时生成波形
 *
 * freq   Frequency in Hertz
 * length Total length in Microseconds
 * cycle  0 ~ 255
 */
void pwm_gen_rt(const PwmGenerator *pwm_gen, uint32_t freq, uint32_t length, uint8_t cycle)
{
  if (cycle == 0)
    cycle = pwm_gen->default_cycle;
  
  float period_full = 1000000 / freq;
  float period_up = (period_full / 255) * cycle;
  float period_down = (period_full / 255) * (1 - cycle);
  uint32_t total_duration = 0;
  while (total_duration / 1000 <= length)
  {
    total_duration += period_full;
    gpio_set(pwm_gen->gpio_port, pwm_gen->gpios);
    udelay(period_up);
    gpio_clear(pwm_gen->gpio_port, pwm_gen->gpios);
    udelay(period_down);
  }
}