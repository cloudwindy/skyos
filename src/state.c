#include "state.h"
#include "mem.h"

State g_st;
VolatileState g_vst;

/**
 * Read only.
 * You're not supposed to modify the global state returned by state().
 */
State *state(void)
{
  return &g_st;
}

/**
 * Read-write.
 */
VolatileState *vstate(void)
{
  return &g_vst;
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