#ifndef SKYRADIO_STATE_H
#define SKYRADIO_STATE_H

#include <stdint.h>

typedef struct state
{
  struct state_ui
  {
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

/**
 * Should not be written to EEPROM.
 */
typedef struct vstate
{
  enum function
  {
    fun_home,
  } fun;
} VolatileState;

State *state(void);
VolatileState *vstate(void);
void state_freq_step_up(void);
void state_freq_step_down(void);

#endif
