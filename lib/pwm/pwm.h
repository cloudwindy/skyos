#ifndef SKYRADIO_PWM_H
#define SKYRADIO_PWM_H

#include <stdint.h>

typedef struct
{
  uint32_t gpio_port;
  uint16_t gpios;
  double default_cycle;
} PwmGenerator;

void pwm_init(const PwmGenerator *pwm_gen);
void pwm_gen_rt(const PwmGenerator *pwm_gen, uint32_t freq, uint32_t length, double cycle);

#endif