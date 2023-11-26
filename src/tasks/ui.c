#include "tasks.h"

#include "ui.h"
#include "app.h"
#include "mem.h"

void task_ui(void *args __attribute__((unused)))
{
  UI *ui = memalloc(sizeof(UI));
  ui_init(ui);
  /* Welcome screen. */
  ui_text(ui, 0, 0, "skyOS");
  ui_line_break(ui, 16);
  ui_update(ui);
  os_delay(1000);

  while (true)
  {
    ui_clear(ui);
    app_update_ui(ui);
    ui_update(ui);
    os_delay(50);
  }
  os_exit();
}