#include "app.h"
#include "state.h"
#include "printf.h"

#include <stdint.h>
#include <string.h>

static void home_update_ui(UI *ui);
static void home_process_key(char key, KeyPress kp, uint32_t hold_time);

void home_handler(EvType type, void *event)
{
  switch (type)
  {
  case ev_ui:
    home_update_ui(event);
    break;
  case ev_key:
    EvKey *evkey = event;
    home_process_key(evkey->key, evkey->key_press, evkey->hold_time);
    break;
  default:
    break;
  }

}

static void home_update_ui(UI *ui)
{
  ui_status_bar(ui);
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

static void home_process_key(char key, enum key_press kp, uint32_t hold_time)
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
