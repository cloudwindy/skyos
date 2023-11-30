#include "tasks.h"

#include "os.h"
#include "app.h"
#include "mem.h"
#include "ui.h"
#include "state.h"

#define UI_TICK 50

void task_ui(void *args __attribute__((unused)))
{
  UI *ui = memalloc(sizeof(UI));
  ui_init(ui);

  while (true)
  {
    ui_clear(ui);
    app_handler(ev_ui, ui);
    ui_update(ui);
    os_delay(50);
  }
  os_exit();
}
