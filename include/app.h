#ifndef SKYRADIO_APP_H
#define SKYRADIO_APP_H

#include <stdbool.h>
#include <stdint.h>

typedef enum key_press
{
  kp_short_press_released,
  kp_long_press_holding,
  kp_long_press_released
} KeyPress;

void app_process_key(char key, KeyPress kp, uint32_t holding_time);

#endif
