#include "app.h"
#include "state.h"
#include "printf.h"

void app_process_key(char key, KeyPress kp, uint32_t holding_time)
{
  (void)holding_time;
  if (kp == kp_short_press_released)
  {
    switch (key)
    {
    case 'b':
      state_freq_step_up();
      break;
    case 'c':
      state_freq_step_down();
      break;
    }
  }
  else
  {
    switch (key)
    {
    case 'b':
      state_freq_step_up();
      break;
    case 'c':
      state_freq_step_down();
      break;
    }
  }
}