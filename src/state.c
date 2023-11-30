#include "state.h"
#include "mem.h"

State g_st;
enum function g_fun = fun_invalid;

/**
 * Read only.
 * You're not supposed to modify the global state returned by state().
 */
State *state(void)
{
  return &g_st;
}

void state_restore_to_defaults(void)
{
  g_st.initialized = true;
  g_st.vfo_freq = 438'500'000;
  g_st.freq_step = 25'000;
  g_st.fs_mode = fs_vfo;
  g_st.ui.hold_delay = 500;
  g_st.ui.fun_default = fun_home;
  g_st.ui.enable_welcome_screen = true;
}

void state_freq_step_up(void)
{
  g_st.vfo_freq += g_st.freq_step;
}

void state_freq_step_down(void)
{
  if (g_st.vfo_freq > g_st.freq_step)
    g_st.vfo_freq -= g_st.freq_step;
}

