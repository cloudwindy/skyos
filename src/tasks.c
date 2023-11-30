#include "app.h"
#include "tasks.h"
#include "state.h"
#include "os.h"
#include "printf.h"

#include <libopencm3/stm32/iwdg.h>

static Task task_stayin_alive;

void task_init(void *args __attribute__((unused)))
{
  printf("skyOS started\n");

  /* TODO: implement state_load */
  if (!g_st.initialized)
  {
    state_restore_to_defaults();
  }

  os_exec("task_ui", task_ui, NULL, 0);
  os_exec("task_keypad", task_keypad, NULL, 0);
  os_exec("task_remote", task_remote, NULL, 0);
  os_exec("task_tick", task_tick, NULL, 0);
  os_exec("task_stayin_alive", task_stayin_alive, NULL, 1);

  if (g_st.ui.welcome_screen_time > 0)
  {
    state_switch_function(fun_welcome);
    os_delay(g_st.ui.welcome_screen_time);
    state_switch_function(g_st.ui.fun_default);
  }

  os_exit();
}

static void task_stayin_alive(void *args __attribute__((unused)))
{
  while (true)
  { /* Life goin' nowhere, somebody help me. */
    iwdg_reset();
    os_delay(50);
  }
}
