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

void state_freq_step_up(void)
{
  g_st.vfo_freq += g_st.freq_step;
}

void state_freq_step_down(void)
{
  if (g_st.vfo_freq > g_st.freq_step)
    g_st.vfo_freq -= g_st.freq_step;
}
