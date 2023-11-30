#include "state.h"
#include "app.h"
#include "mem.h"

/**
 * Read only.
 * You're not supposed to modify the global state.
 * If you need to, call state_*().
 */
State g_st;
Function g_fun = fun_invalid;

void state_restore_to_defaults(void)
{
  g_st.initialized = true;
  g_st.vfo_freq = 438'500'000;
  g_st.freq_step = 25'000;
  g_st.fs_mode = fs_vfo;
  g_st.ui.hold_delay = 500;
  g_st.ui.fun_default = fun_home;
  g_st.ui.extra_boot_time = 500;
}

void state_switch_function(Function fun)
{
  app_handler(ev_clean, NULL);
  g_fun = fun;
  app_handler(ev_init, NULL);
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

