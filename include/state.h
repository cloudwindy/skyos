#ifndef SKYRADIO_STATE_H
#define SKYRADIO_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct state
{
  struct state_ui
  {
    bool enable_welcome_screen;
    enum function
    {
      fun_home,
      fun_fmradio,
      fun_settings,
      fun_invalid
    } fun_default;
    uint32_t hold_delay;
  } ui;
  uint32_t freq_step;
  uint32_t vfo_freq;
  enum freq_selector_mode
  {
    fs_vfo,
    fs_mr
  } fs_mode;
} State;

extern enum function g_fun;

State *state(void);
void state_freq_step_up(void);
void state_freq_step_down(void);

#endif
