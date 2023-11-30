#include "app.h"
#include "mem.h"
#include "state.h"
#include "printf.h"
#include "ui.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define FREQ_STR_SIZE (16 + 1)
#define EDIT_BUF_SIZE (9 + 1)

static void init(void);
static void clean(void);
static void timeout(void);
static void update_ui(UI *ui);
static void process_key(char key, KeyPress kp, uint32_t hold_time);

char *freq_str;

bool editing;
char *edit_buf;
uint8_t edit_cur;

uint16_t idle_time = 0;

void home_handler(EvType type, void *event)
{
  switch (type)
  {
  case ev_ui:
    update_ui(event);
    break;
  case ev_tick:
    if (idle_time > 5000)
    {
      timeout();
      idle_time = 0;
    }
    else
    {
      idle_time += APP_TICK;
    }
    break;
  case ev_key:;
    EvKey *evkey = event;
    process_key(evkey->key, evkey->key_press, evkey->hold_time);
    break;
  case ev_init:
    init();
    break;
  case ev_clean:
    clean();
    break;
  default:
    break;
  }
}

static void init(void)
{
  editing = false;
  freq_str = memalloc(FREQ_STR_SIZE);
  edit_buf = memalloc(EDIT_BUF_SIZE);
  edit_cur = 0;
}

static void clean(void)
{
  memfree(freq_str);
  memfree(edit_buf);
}

static void update_ui(UI *ui)
{
  ui_time(ui);
  ui_line_break(ui, 16);
  switch (g_st.fs_mode)
  {
  case fs_vfo:
    strcpy(freq_str, " VFO ");
    if (!editing)
    {
      snprintf(&freq_str[5], FREQ_STR_SIZE - 5, "%3d.%06d",
               g_st.vfo_freq / 1000000, g_st.vfo_freq % 1000000);
    }
    else
    {
      snprintf(&freq_str[5], FREQ_STR_SIZE - 5, "%.3s.%s",
               edit_buf, &edit_buf[3]);
    }
    break;
  case fs_mr:
    strcpy(freq_str, " MR ");
    break;
  }
  ui_text(ui, 0, 2, freq_str);
}

static void process_key(char key, KeyPress kp, uint32_t hold_time)
{
  if (kp == kp_pressed)
  { /* Ignore key press event. */
    return;
  }
  if (kp == kp_short_released && key >= '0' && key <= '9')
  { /* Edit frequency. */
    if (!editing)
    {
      editing = true;
      memset(edit_buf, '-', EDIT_BUF_SIZE);
      edit_buf[EDIT_BUF_SIZE - 1] = '\0';
    }
    edit_buf[edit_cur] = key;
    edit_cur++;
    if (edit_cur == EDIT_BUF_SIZE - 1)
    {
      editing = false;
      edit_cur = 0;
    }
  }
  else
  {
    timeout();
    if (key == 'b')
    {
      state_freq_step_up();
    }
    else if (key == 'c')
    {
      state_freq_step_down();
    }
    else if (kp == kp_long_holding && hold_time == 0)
    { /* This is a function key. */
      switch (key)
      {
      case 'a':
        state_switch_function(fun_settings);
        break;
      }
    }
  }
  idle_time = 0;
}

static void timeout(void)
{
  editing = false;
  edit_cur = 0;
}

