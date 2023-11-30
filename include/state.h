#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
  fun_home,
  fun_fmradio,
  fun_settings,
  fun_invalid
} Function;

/* Frequency Selector Mode */
typedef enum
{
  fs_vfo,
  fs_mr
} FSMode;

typedef struct
{
  uint16_t welcome_screen_time;
  Function fun_default;
  uint32_t hold_delay;
} StateUI;

typedef struct state
{
  bool initialized;
  StateUI ui;
  uint32_t freq_step;
  uint32_t vfo_freq;
  FSMode fs_mode;
} State;

extern State g_st;
extern Function g_fun;

void state_restore_to_defaults(void);
void state_switch_function(Function fun);
void state_freq_step_up(void);
void state_freq_step_down(void);

#endif
