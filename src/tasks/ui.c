#include "tasks.h"

#include "app.h"
#include "mem.h"
#include "ui.h"
#include "state.h"

void task_ui(void *args __attribute__((unused)))
{
  UI *ui = memalloc(sizeof(UI));
  ui_init(ui);
  if (g_st.ui.welcome_screen_time > 0)
  {
    /* Welcome screen. */
    ui_text(ui, 0, 0, "skyOS");
    ui_line_break(ui, 16);
    ui_update(ui);
    os_delay(g_st.ui.welcome_screen_time);
  }
  state_switch_function(g_st.ui.fun_default);

  while (true)
  {
    ui_clear(ui);
    app_handler(ev_ui, ui);
    ui_update(ui);
    os_delay(50);
  }
  os_exit();
}
