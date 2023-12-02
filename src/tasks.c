#include "tasks.h"
#include "app.h"
#include "os.h"
#include "printf.h"
#include "state.h"

#include <libopencm3/stm32/iwdg.h>

void task_init(void *args __attribute__((unused)))
{
  printf("skyOS started\n");

  /* TODO: implement state_load */
  if (!g_st.initialized)
  {
    state_restore_to_defaults();
  }

  os_exec("ui", task_ui, NULL, 0);
  os_exec("keypad", task_keypad, NULL, 0);
  os_exec("remote", task_remote, NULL, 0);
  os_exec("tick", task_tick, NULL, 0);
  os_exec("watchdog", task_watchdog, NULL, 1);

  state_switch_function(fun_boot);
  if (g_st.ui.extra_boot_time > 0)
  {
    os_delay(g_st.ui.extra_boot_time);
  }
  state_switch_function(g_st.ui.fun_default);

  os_exit();
}
