#ifndef PWM_H
#define PWM_H

#include <stdint.h>

typedef struct pwm_generator
{
  uint32_t gpio_port;
  uint16_t gpios;
  uint32_t low_freq;
  uint32_t high_freq;
  double default_cycle;
} PwmGenerator;

void pwm_init(const PwmGenerator *pwm_gen);
void pwm_gen_rt(const PwmGenerator *pwm_gen, uint32_t freq, uint32_t length, double cycle);
void pwm_send(const PwmGenerator *pwm_gen, uint8_t byte, uint32_t length_of_bit, uint32_t length_between_bit);

#endif