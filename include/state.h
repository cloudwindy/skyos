#ifndef SKYRADIO_STATE_H
#define SKYRADIO_STATE_H

#include <stdint.h>

typedef enum freq_selector_mode
{
  fs_vfo,
  fs_mr
} FSMode;

typedef struct state
{
  struct state_ui {
    uint32_t hold_delay;
  } ui;
  uint32_t freq_step;
  uint32_t vfo_freq;
  FSMode fs_mode;
} State;

State *state(void);
void state_freq_step_up(void);
void state_freq_step_down(void);

#endif
