#include "app.h"
#include "state.h"
#include "printf.h"

#include <string.h>

void home_update_ui(UI *ui)
{
  char freq_str[16 + 1];
  State *st = state();
  switch (st->fs_mode)
  {
  case fs_vfo:
    snprintf(freq_str, sizeof(freq_str),
             " VFO %3d.%06d", st->vfo_freq / 1000000,
             st->vfo_freq % 1000000);
    break;
  case fs_mr:
    strcpy(freq_str, " MR ");
    break;
  }
  ui_text(ui, 0, 2, freq_str);
}

void home_process_key(char key, KeyPress kp, uint32_t hold_time)
{
  (void)hold_time;
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