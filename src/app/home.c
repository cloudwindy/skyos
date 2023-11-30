#include "app.h"
#include "state.h"
#include "printf.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static void home_update_ui(UI *ui);
static void home_process_key(char key, KeyPress kp, uint32_t hold_time);

bool editing = false;
char edit_buf[9 + 1];
uint8_t edit_cur = 0;

void home_handler(EvType type, void *event)
{
  switch (type)
  {
  case ev_ui:
    home_update_ui(event);
    break;
  case ev_key:
    const EvKey *evkey = event;
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
  const State *st = state();
  switch (st->fs_mode)
  {
  case fs_vfo:
    strcpy(freq_str, " VFO ");
    if (!editing)
    {
      snprintf(&freq_str[5], sizeof(freq_str) - 5, "%3d.%06d",
               st->vfo_freq / 1000000, st->vfo_freq % 1000000);
    }
    else
    {
      snprintf(&freq_str[5], sizeof(freq_str) - 5, "%.3s.%s",
               edit_buf, &edit_buf[3]);
    }
    break;
  case fs_mr:
    strcpy(freq_str, " MR ");
    break;
  }
  ui_text(ui, 0, 2, freq_str);
}

static void home_process_key(char key, KeyPress kp, uint32_t hold_time)
{
  (void)kp;
  (void)hold_time;
  if (kp == kp_short_press_released && key >= '0' && key <= '9')
  {
    if (!editing)
    {
      editing = true;
      memset(edit_buf, '-', sizeof(edit_buf));
      edit_buf[sizeof(edit_buf) - 1] = '\0';
    }
    edit_buf[edit_cur] = key;
    edit_cur++;
    if (edit_cur == sizeof(edit_buf) - 1)
    {
      editing = false;
      edit_cur = 0;
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
